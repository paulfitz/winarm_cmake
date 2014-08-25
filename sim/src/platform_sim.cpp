//extern "C" {
#include "LPC214x.h"
//}

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <yarp/os/Time.h>
#include <yarp/os/Network.h>
#include <yarp/os/BufferedPort.h>
#include <yarp/os/Property.h>
#include <string>
#include <map>
#include <bot_bot.h>
using namespace yarp::os;
using namespace std;

static map<string,long int> used;
static map<string,BasicSlot *> forwarder;

class InterruptSlot : public BasicSlot {
public:
  unsigned long raw;
  InterruptSlot() { raw = 0; }

  virtual unsigned long get_int() {
    return raw;
  }
  virtual void set_int(unsigned long x) {
    raw = raw & (~x);  // interrupt stuff seems to be like this?
  }
  virtual void set_raw(unsigned long x) {
    raw = x;
  }
};

class SetSlot : public BasicSlot {
public:
  int *target;
  int *mask;
  int sgn;
  SetSlot() { target = 0; sgn = 0; }
  void init (int& target, int& mask, int& dir, int sgn) {
    this->target = &target;
    this->mask = &mask;
    this->sgn = sgn;
  }
  virtual unsigned long get_int() {
    if (target) return ((*target) & (~(*mask)));
    return 0;
  } 
 virtual void set_int(unsigned long x) {
   if (target) {
     if (sgn==1) {
       *target |= (x & (~(*mask)));
     } else if (sgn==0) {
       *target &= ~(x & (~(*mask)));
     } else {
       *target = ((*target)&(*mask)) | (x & (~(*mask)));
     }
   }
  }
};

class DumbSlot : public BasicSlot {
public:
  int *target;
  DumbSlot() { target = 0; }
  void init (int& target) {
    this->target = &target;
  }
  virtual unsigned long get_int() {
    if (target) return *target;
    return 0;
  } 
  virtual void set_int(unsigned long x) {
    if (target) *target = x;
  }
};


class SpiDataSlot : public BasicSlot {
public:
  unsigned long raw;
  int ct;
  SpiDataSlot() { raw = 0; ct = -1; }
  virtual unsigned long get_int() {
    return raw;
  } 
  virtual void set_int(unsigned long x) {
    raw = x;
    // initiate transfer...
    ct = 0;
  }
  int count() {
    if (ct>=0) ct++;
    return ct;
  }
  void reset() { ct = -1; }
};

class SpiStatusSlot : public BasicSlot {
public:
  unsigned long raw;
  SpiStatusSlot() { raw = 0; }
  virtual unsigned long get_int() {
    unsigned int result = raw;
    raw = 0;
    return result;
  } 
  virtual void set_int(unsigned long x) {
    printf("Do not write to Spi Status\n");
    assert(1==0);
  }  
};

class State {
public:
  int dout[2];
  int dmask[2];
  int ddir[2];
  InterruptSlot tick;
  SetSlot dout_onoff[2][3];
  DumbSlot dout_dir[2];
  DumbSlot dout_mask[2];
  SpiStatusSlot spi_status;
  SpiDataSlot spi_data;

  State() {
    for (int i=0; i<2; i++) {
      dout[i] = 0;
      dmask[i] = 0;
      ddir[i] = 0;
    }
    for (int i=0; i<2; i++) {
      for (int j=0; j<3; j++) {
	dout_onoff[i][j].init(dout[i],dmask[i],ddir[i],j);
      }
      dout_dir[i].init(ddir[i]);
      dout_mask[i].init(dmask[i]);
    }
    forwarder["T0IR"] = &tick;
    forwarder["FIO0CLR"] = &(dout_onoff[0][0]);
    forwarder["FIO0SET"] = &(dout_onoff[0][1]);
    forwarder["FIO0PIN"] = &(dout_onoff[0][2]);
    forwarder["FIO0DIR"] = &(dout_dir[0]);
    forwarder["FIO0MASK"] = &(dout_mask[0]);
    forwarder["FIO1CLR"] = &(dout_onoff[1][0]);
    forwarder["FIO1SET"] = &(dout_onoff[1][1]);
    forwarder["FIO1PIN"] = &(dout_onoff[1][2]);
    forwarder["FIO1DIR"] = &(dout_dir[1]);
    forwarder["FIO1MASK"] = &(dout_mask[1]);
    forwarder["S0SPDR"] = &spi_data;
    forwarder["S0SPSR"] = &spi_status;
  }

  void update() {
    if (spi_data.count()>2) {
      spi_data.reset();
      spi_status.raw = 0x80;
    }
  }
};


unsigned long *device_base_addr = 0;
static SniffSlot *slots = 0;
static int hack_active = 1;
static State state;

static void set_switch(const char *name, Property& p, int& target, int& msk, int msk0) {
  Value& v = p.find(name);
  if (v.isNull()) return;
  msk |= msk0;
  if (v.asInt()>0) {
    target |= msk0;
  }
}

static void update() {
  static Network yarp;
  static BufferedPort<Property> port;
  static bool opened = false;
  if (!opened) {
    if (!port.open("/bot")) exit(1);
    opened = true;
    port.setStrict(true);
  }
  hack_active = 0;
  state.update();
  T0IR.set_raw(1);
  U0LSR = 0x20;
  PLLSTAT = (1<<10);
  int v = state.dout[0];

  static int freq = 0;
  freq++;
  if (freq==100) {
    freq = 0;

    Property& p = port.prepare();
    p.put("p0_led", (v & P0_LED)?1:0);
    p.put("p1_led_0", (v & P1_LED_0)?1:0);
    p.put("p1_led_1", (v & P1_LED_1)?1:0);
    p.put("p0_led_2", (v & P0_LED_2)?1:0);
    p.put("p1_led_lw", (v & P1_LED_LW)?1:0);
    p.put("p0_led_rw", (v & P0_LED_RW)?1:0);
    p.put("s0", (v & SW_0)?1:0);
    p.put("s1", (v & SW_1)?1:0);
    printf("STATE %s\n", p.toString().c_str());
    port.write();

    while (true) {
      Property *pi = port.read(false);
      if (!pi) break;
      int target = 0;
      int msk = 0;
      set_switch("s0",*pi,target,msk,SW_0);
      set_switch("s1",*pi,target,msk,SW_1);
      if (msk!=0) {
	int& x = state.dout[0];
	x = (x&(~msk)) | target;
      }
    }
  }

#if 0
  printf("UPDATE %ld\n", state.dout[0]);
  printf("USED: ");
  for (map<string,long int>::iterator it = used.begin(); it != used.end(); it++) {
    bool known = forwarder.find(it->first)!=forwarder.end();
    printf("%s%s:%ld ", known?"":"!", it->first.c_str(), it->second);
  }
  printf("\n");
#endif
  hack_active = 1;
}

static void check() {
  static double t0 = Time::now();
  double t = Time::now();
  double step = 0.00005;
  if (t-t0 > step) {
    update();
    t0 += step;
  }
}

unsigned long *setup_device() {
  if (!device_base_addr) {
    printf("ALLOCATE\n");
    int n = 0x4000;
    device_base_addr = new unsigned long[n];
    memset((void*)device_base_addr,0,n);
    slots = new SniffSlot[n];
  }
  if (hack_active) check();
  return device_base_addr;
}

SniffSlot& define_address0(const char *name, int base, int offset, const char *fname, int line) {
  if (hack_active) {
    printf("LOCATION: %s %d\n", fname, line);
  }
  setup_device();
  SniffSlot& slot = slots[base+offset];
  if (slot.name[0] == '\0') {
    strncpy(slot.name,name,sizeof(slot.name));
    slot.name[sizeof(slot.name)-1] = '\0';
    used[name] = 0;
    if (forwarder.find(name)!=forwarder.end()) {
      printf("SET FORWARDER: %s\n", name, fname, line);
      slot.fwd = forwarder[name];
    }
  }
  used[name]++;
  //Time::delay(0.0005); // throttle
  return slot;
}

State& get_state() {
  return state;
}




unsigned long SniffSlot::get_int() { 
  printf("ACCESS Read %s\n", name);
  if (fwd) {
    return fwd->get_int();
  };
  return raw;
}

void SniffSlot::set_int(unsigned long x) {
  printf("ACCESS Set %s to 0x%x\n", name, x);
  if (fwd) {
    fwd->set_int(x);
  } else {
    raw = x;
  }
}

void SniffSlot::set_raw(unsigned long x) {
  printf("ACCESS Set %s to 0x%x (raw)\n", name, x);
  if (fwd) {
    fwd->set_raw(x);
  } else {
    raw = x;
  }
}
