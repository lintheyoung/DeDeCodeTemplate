#include "SimpleFSM.h"

typedef void (*StateAction)();

class FSMManager {
private:
  SimpleFSM fsm;
  State* initialState;
  State* states;
  Transition* transitions;
  int numStates;
  int numTransitions;
  int nextTrigger;

public:
  FSMManager(): initialState(nullptr), states(nullptr), transitions(nullptr), numStates(0), numTransitions(0), nextTrigger(0) {}

  ~FSMManager() {
    delete initialState;
    delete[] states;
    delete[] transitions;
  }

  void setup(StateAction introActions[], StateAction runActions[], StateAction outroActions[], int stateCount) {
    // 清理旧资源
    delete initialState;
    delete[] states;
    delete[] transitions;

    numStates = stateCount;
    states = new State[numStates];

    // 初始化状态
    for (int i = 0; i < numStates; i++) {
      states[i] = State("State" + String(i + 1), introActions[i], runActions[i], outroActions[i]);
    }

    // 初始化转换逻辑
    numTransitions = numStates + (numStates - 1) * numStates;
    transitions = new Transition[numTransitions];
    initialState = new State("InitialState", []() {});
    int t = 0;

    // 填充转换逻辑
    for (int i = 0; i < numStates; i++) {
      transitions[t++] = Transition(initialState, &states[i], i + 1);
      for (int j = 0; j < numStates; j++) {
        if (i != j) {
          transitions[t++] = Transition(&states[i], &states[j], j + 1);
        }
      }
    }

    fsm.add(transitions, numTransitions);
    fsm.setInitialState(initialState);
  }

  void trigger(int t) {
    nextTrigger = t;
  }

  void reset() {
    // 空，支持之前的接口
  }

  void run(int delayTime) {
    fsm.run(delayTime);
    if (nextTrigger != 0) {
      fsm.trigger(nextTrigger);
      nextTrigger = 0;
    }
  }

  void run(int delayTime, CallbackFunction tick_cb) {
    fsm.run(delayTime, tick_cb);
    if (nextTrigger != 0) {
      fsm.trigger(nextTrigger);
      nextTrigger = 0;
    }
  }
};

FSMManager fsmManager;

// 示例：用户自定义的函数
void customIntro1() {
  Serial.println("Custom Intro 1");
}
void customRun1() {
  Serial.println("Custom Run 1");
}
void customOutro1() {
  Serial.println("Custom Outro 1");
}

void customIntro2() {
  Serial.println("Custom Intro 2");
}
void customRun2() {
  Serial.println("Custom Run 2");
}
void customOutro2() {
  Serial.println("Custom Outro 2");
}

void customIntro3() {
  Serial.println("Custom Intro 3");
}
void customRun3() {
  Serial.println("Custom Run 3");
}
void customOutro3() {
  Serial.println("Custom Outro 3");
}

void customIntro4() {
  Serial.println("Custom Intro 4");
}
void customRun4() {
  Serial.println("Custom Run 4");
}
void customOutro4() {
  Serial.println("Custom Outro 4");
}

void customIntro5() {
  Serial.println("Custom Intro 5");
}
void customRun5() {
  Serial.println("Custom Run 5");
  fsmManager.trigger(1);
}
void customOutro5() {
  Serial.println("Custom Outro 5");
  
}

void customIntro6() {
  Serial.println("Custom Intro 6");
}
void customRun6() {
  Serial.println("Custom Run 6");
}
void customOutro6() {
  Serial.println("Custom Outro 6");
  
}



void setup() {
  Serial.begin(115200);
  delay(1000);

  // 为每个状态创建自定义函数数组
  StateAction intros[6] = { customIntro1, customIntro2, customIntro3, customIntro4, customIntro5, customIntro6 };
  StateAction runs[6] = { customRun1, customRun2, customRun3, customRun4, customRun5, customRun6 };
  StateAction outros[6] = { customOutro1, customOutro2, customOutro3, customOutro4, customOutro5, customOutro6 };

  // 创建FSMManager实例
  fsmManager.setup(intros, runs, outros, 6);

  Serial.println("FSM Ready");
}

void loop() {
  // 运行状态机逻辑
  fsmManager.run(5);

  // 处理串口命令
  if (Serial.available() > 0) {
    int command = Serial.read() - '0';
    if (command >= 1 && command <= 6) {
      fsmManager.trigger(command);
    }
  }
}
