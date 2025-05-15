# CRA_TeamBest_SSDShell
CRA Team Best Shell Program Repository 입니다.


# coding style
- 변수명은 소문자로 시작, 카멜 방식 : nameTable, ageGroup
- const 변수명은 대문자와_ 로 사용 : MAX_COUNT
- 함수명은 대문자로 시작, 동사+명사형태 : SetName()
- 파일명은 소문자와_ 사용 : ssd_write_program.cpp
- 중괄호는 함수 바로 옆에 시작
   void SetName(string str) {
   
# Commit Message Guide
-  [Feature or Refactor] 해당 Feature

- 변경점  

- 검증
 
   [ ] Build

   [ ] Regression Test Pass

   [ ] 해당 기능 TC

# 실행 방법
1. Debug mode 는 빌드 -> 실행하면 자동으로 main() 함수에서 RUN_ALL_TESTS(); 가 실행됨
2. Release mode 는 빌드 -> 실행 하면 자동으로 SSDShell 이 Run() 이 돌아가며 exit 를 입력하기 전까지는 계속 프롬프트 창에 Shell> 이 떠 있다
![image](https://github.com/user-attachments/assets/f55eee67-8bb9-490f-b752-ccd1a83800b8)
3. Release mode 로 빌드하면 \x64\Release 아래 TeamBest_SSD_Shell.exe 가 생성된다. cmd 창에 TeamBest_SSD_Shell.exe shell_script.txt 라고 입력하면 shell_script.txt 에 들어있는 tc 이름을 찾아 해당 tc 가 순차적으로 수행된다.
  ![image](https://github.com/user-attachments/assets/c57f8efb-1f16-487b-9bec-6dfcd0cce2b7)

# Shell 의 Visual studio project 구성
![image](https://github.com/user-attachments/assets/7bf8afdf-fe32-4607-a994-67b919d418fb)
 - ScriptImplementDLL : 동적 라이브러리 프로젝트. 원래는 최소한의 빌드만 하도록 script 를 dll 로 만들기 위해 만들었으나 최종 완성은 못함
 - ScriptInterfaceLib : 정적 라이브러리 프로젝트. 이것도 최종 완성은 못함. 공통부 코드가 들어가려고 했던 곳임
 - TeamBest_SSD_Shell : Shell, Logger, Runner 및 TC 가 포함된 exe 용 프로젝트

# Shell 프로젝트 실행을 하기 위한 파일 구성
![image](https://github.com/user-attachments/assets/236a621a-868d-4b62-8d5a-4ccd50dc9bd5)
- TeamBest_SSD_Shell.exe : Shell 실행 파일
- ssd.exe 는 SSD 프로젝트에서 생성된 결과물임
- shell_script.txt : TeamBest_SSD_Shell.exe 가 실행할 때 필요한 script 목록이 적힌 파일
- ssd_nand.txt / ssd_output.txt 는 ssd.exe 가 실행되면서 생성됨
- logs 폴더는 shell 이 동작하면서 LOG_MESSAGE() 의 내용이 기록됨
- ![image](https://github.com/user-attachments/assets/37506a78-0801-48db-82cf-adebdddd2cac)

- buffer 폴더는 ssd.exe 가 동작하면서 생성됨
- ![image](https://github.com/user-attachments/assets/9284d844-46b5-49ae-af7b-8c3e849ea8f8)



