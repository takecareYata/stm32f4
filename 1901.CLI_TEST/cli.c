#include "device_driver.h"
#include <stdio.h>
#include <string.h>

// LED 토글 보조 함수


// LED 서브 커맨드 구조체 정의
typedef struct {
    const char *cmd;       // 서브 명령어 ("on", "off" 등)
    void (*func)(void);    // 실행 함수 포인터
    const char *desc;      // 도움말 설명
} Cmd_t;

// LED 서브 커맨드 테이블 (명령어 추가 시 이 배열에 1줄 추가)
static const Cmd_t led_cmd_table[] = {
    { "on",     LED_On,     "Turn ON the LED" },
    { "off",    LED_Off,    "Turn OFF the LED" },
    { "toggle", LED_Toggle, "Toggle the LED state" }
};

static const Cmd_t servo_cmd_table[] = {
    { "on",     Servo_Set90,     "Turn ON Servo" },
    { "off",    Servo_Set0,    "Turn OFF Servo" },
};

static const Cmd_t check_cmd_table[] = {
    { "blue",     Check_Blue,     "Blue color checked" },
    { "red",    Check_Red,    "Red color checked" },
    { "stop",    Check_Stop,    "color checking STOP" },
};

#define LED_CMD_COUNT (sizeof(led_cmd_table) / sizeof(led_cmd_table[0]))
#define SERVO_CMD_COUNT (sizeof(servo_cmd_table) / sizeof(servo_cmd_table[0]))
#define CHECK_CMD_COUNT (sizeof(check_cmd_table) / sizeof(check_cmd_table[0]))

// LED 도움말 출력
static void Show_Led_Help(void) 
{
    printf("\n=== [LED Control Commands] ===\n");
    for (int i = 0; i < LED_CMD_COUNT; i++) {
        printf("  led %-8s : %s\n", led_cmd_table[i].cmd, led_cmd_table[i].desc);
    }
    printf("==============================\n\n");
}

// LED 명령어 핸들러
static void Cmd_Led(int argc, char *argv[]) 
{
    if (argc < 2) {
        Show_Led_Help();
        return;
    }

    for (int i = 0; i < LED_CMD_COUNT; i++) {
        if (strcmp(argv[1], led_cmd_table[i].cmd) == 0) {
            led_toggle_flag = 0;
            led_cmd_table[i].func();
            printf("[CLI] LED %s executed.\n", led_cmd_table[i].cmd);
            return;
        }
    }

    printf("[CLI] Unknown option '%s'. Type 'led' for help.\n", argv[1]);
}

static void Show_Servo_Help(void) 
{
    printf("\n=== [servo Control Commands] ===\n");
    for (int i = 0; i < SERVO_CMD_COUNT; i++) {
        printf("  servo %-8s : %s\n", servo_cmd_table[i].cmd, servo_cmd_table[i].desc);
    }
    printf("==============================\n\n");
}

// Servo 명령어 핸들러
static void Cmd_Servo(int argc, char *argv[]) 
{
    if (argc < 2) {
        Show_Servo_Help();
        return;
    }

    for (int i = 0; i < SERVO_CMD_COUNT; i++) {
        if (strcmp(argv[1], servo_cmd_table[i].cmd) == 0) {
            servo_cmd_table[i].func();
            printf("[CLI] Servo %s executed.\n", servo_cmd_table[i].cmd);
            return;
        }
    }

    printf("[CLI] Unknown option '%s'. Type 'servo' for help.\n", argv[1]);
}

static void Show_Check_Help(void) 
{
    printf("\n=== [Check Color Commands] ===\n");
    for (int i = 0; i < CHECK_CMD_COUNT; i++) {
        printf("  check %-8s : %s\n", check_cmd_table[i].cmd, check_cmd_table[i].desc);
    }
    printf("==============================\n\n");
}

// Servo 명령어 핸들러
static void Cmd_Check(int argc, char *argv[]) 
{
    if (argc < 2) {
        Show_Check_Help();
        return;
    }

    for (int i = 0; i < CHECK_CMD_COUNT; i++) {
        if (strcmp(argv[1], check_cmd_table[i].cmd) == 0) {
            led_toggle_flag = 0;
            check_cmd_table[i].func();
            printf("[CLI] check %s executed.\n", check_cmd_table[i].cmd);
            return;
        }
    }

    printf("[CLI] Unknown option '%s'. Type 'check' for help.\n", argv[1]);
}

// 최상위 CLI 라인 파서
void Process_CLI_Line(char *line) 
{
    int argc = 0;
    char *argv[MAX_ARGC];

    char *token = strtok(line, " \t\r\n");
    while (token != NULL && argc < MAX_ARGC) {
        argv[argc++] = token;
        token = strtok(NULL, " \t\r\n");
    }

    if (argc == 0) return;

    if (strcmp(argv[0], "led") == 0) 
    {
        Cmd_Led(argc, argv);
    }
    else if (strcmp(argv[0], "servo") == 0)
    {
        Cmd_Servo(argc,argv);
    }
    else if (strcmp(argv[0], "check") == 0)
    {
        Cmd_Check(argc,argv);
    }
    else 
    {
        printf("[CLI] Unknown command: '%s'. Type 'led','servo','check' for help.\n", argv[0]);
    }
}