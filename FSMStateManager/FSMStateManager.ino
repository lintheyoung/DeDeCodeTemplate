#include "SimpleFSM.h"

// 定义一个函数指针类型，用于状态动作
typedef void (*StateAction)();

class FSMManager {
private:
    SimpleFSM fsm;
    State initialState;
    State states[5];
    Transition* transitions;
    int numTransitions;
    int nextTrigger; // 存储下一个触发器的变量

public:
    // 无参构造函数
    FSMManager() : initialState("InitialState", []() {}), numTransitions(0), transitions(nullptr), nextTrigger(0) {}

    // 析构函数
    ~FSMManager() {
        delete[] transitions;
    }

    // 设置状态和转换的方法
    void setup(StateAction introActions[5], StateAction runActions[5], StateAction outroActions[5]) {
        // 初始化状态
        for (int i = 0; i < 5; i++) {
            states[i] = State("State" + String(i + 1), introActions[i], runActions[i], outroActions[i]);
        }

        // 初始化转换逻辑
        numTransitions = 5 + 1 * 4 * 5;  // 5个初始转换 + 20个交叉转换
        delete[] transitions;  // 清除之前的转换（如果有）
        transitions = new Transition[numTransitions];
        int t = 0;

        // 填充转换逻辑
        for (int i = 0; i < 5; i++) {
            transitions[t++] = Transition(&initialState, &states[i], i + 1);
            for (int j = 0; j < 5; j++) {
                if (i != j) {
                    transitions[t++] = Transition(&states[i], &states[j], j + 1);
                }
            }
        }

        fsm.add(transitions, numTransitions);
        fsm.setInitialState(&initialState);
    }

    // 触发器方法
    void trigger(int t) {
        nextTrigger = t; // 设置下一个触发器
    }

    // 运行状态机
    void run(int delayTime) {
        fsm.run(delayTime);

        // 检查是否有待处理的触发器
        if (nextTrigger != 0) {
            fsm.trigger(nextTrigger);
            nextTrigger = 0; // 重置触发器
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



void setup() {
  Serial.begin(115200);
  delay(1000);

  // 为每个状态创建自定义函数数组
  StateAction intros[5] = { customIntro1, customIntro2, customIntro3, customIntro4, customIntro5 };
  StateAction runs[5] = { customRun1, customRun2, customRun3, customRun4, customRun5 };
  StateAction outros[5] = { customOutro1, customOutro2, customOutro3, customOutro4, customOutro5 };

  // 创建FSMManager实例
  fsmManager.setup(intros, runs, outros);

  Serial.println("FSM Ready");
}

void loop() {
  // 运行状态机逻辑
  fsmManager.run(5);

  // 处理串口命令
  if (Serial.available() > 0) {
    int command = Serial.read() - '0';
    if (command >= 1 && command <= 5) {
      fsmManager.trigger(command);
    }
  }
}
