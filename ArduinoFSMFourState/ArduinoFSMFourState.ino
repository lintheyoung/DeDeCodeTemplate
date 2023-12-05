#include "SimpleFSM.h"

SimpleFSM fsm;

// 枚举触发器
enum triggers {
  trigger1 = 1,
  trigger2 = 2,
  trigger3 = 3,
  trigger4 = 4,
  trigger5 = 5
};

// 假设的函数定义
void fun1_intro() { Serial.println("fun1_intro"); }
void fun1_run() { Serial.println("fun1_run"); }
void fun1_outro() { Serial.println("fun1_outro"); }

void fun2_intro() { Serial.println("fun2_intro"); }
void fun2_run() { Serial.println("fun2_run"); }
void fun2_outro() { Serial.println("fun2_outro"); }

void fun3_intro() { Serial.println("fun3_intro"); }
void fun3_run() { Serial.println("fun3_run"); }
void fun3_outro() { Serial.println("fun3_outro"); }

void fun4_intro() { Serial.println("fun4_intro"); }
void fun4_run() { Serial.println("fun4_run"); }
void fun4_outro() { Serial.println("fun4_outro"); }

void fun4_intro() { Serial.println("fun4_intro"); }
void fun4_run() { Serial.println("fun4_run"); }
void fun4_outro() { Serial.println("fun4_outro"); }

void fun5_intro() { Serial.println("fun5_intro"); }
void fun5_run() { Serial.println("fun5_run"); }
void fun5_outro() { Serial.println("fun5_outro"); }

void onInitialState() {
  // 初始状态下不执行任何操作
}

// 定义状态
State initialState("InitialState", onInitialState);
State state1("State1", fun1_intro, fun1_run, fun1_outro);
State state2("State2", fun2_intro, fun2_run, fun2_outro);
State state3("State3", fun3_intro, fun3_run, fun3_outro);
State state4("State4", fun4_intro, fun4_run, fun4_outro);
State state5("State5", fun5_intro, fun5_run, fun5_outro);

// 定义状态转换
Transition transitions[] = {
  Transition(&initialState, &state1, trigger1),
  Transition(&initialState, &state2, trigger2),
  Transition(&initialState, &state3, trigger3),
  Transition(&initialState, &state4, trigger4),
  Transition(&initialState, &state5, trigger5),

  // 1 -> 2; 1 -> 3; 1 -> 4; 1 -> 5;
  // 2 -> 1; 3 -> 1; 4 -> 1; 5 -> 1;
  Transition(&state1, &state2, trigger2),
  Transition(&state2, &state1, trigger1),

  Transition(&state1, &state3, trigger3),
  Transition(&state3, &state1, trigger1),

  Transition(&state1, &state4, trigger4),
  Transition(&state4, &state1, trigger1),

  Transition(&state1, &state5, trigger5),
  Transition(&state5, &state1, trigger1),

  // 2 -> 3; 2 -> 4; 2 -> 5;
  // 3 -> 2; 4 -> 2; 5 -> 2;
  Transition(&state2, &state3, trigger3),
  Transition(&state3, &state2, trigger2),

  Transition(&state2, &state4, trigger4),
  Transition(&state4, &state2, trigger2),

  Transition(&state2, &state5, trigger5),
  Transition(&state5, &state2, trigger2),

  // 3 -> 4; 3 -> 5;
  // 4 -> 3; 5 -> 3;
  Transition(&state3, &state4, trigger4),
  Transition(&state4, &state3, trigger3),

  Transition(&state3, &state5, trigger5),
  Transition(&state5, &state3, trigger3),

  // 4 -> 5;
  // 5 -> 4;
  Transition(&state4, &state5, trigger5),
  Transition(&state5, &state4, trigger4)
};

void setup() {
  Serial.begin(115200);
  delay(1000);
  // 初始化FSM
  fsm.add(transitions, sizeof(transitions) / sizeof(Transition));
  fsm.setInitialState(&initialState); // 设置初始状态

  Serial.println("FSM Ready");
}

void loop() {
  fsm.run(500); // 运行状态机

  if (Serial.available() > 0) {
    int command = Serial.read();
    if (command == '1') {
      fsm.trigger(trigger1);
    } else if (command == '2') {
      fsm.trigger(trigger2);
    } else if (command == '3') {
      fsm.trigger(trigger3);
    } else if (command == '4') {
      fsm.trigger(trigger4);
    } else if (command == '5') {
      fsm.trigger(trigger5);
    }
  }
}