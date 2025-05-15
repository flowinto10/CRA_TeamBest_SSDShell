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
3. Release mode 로 빌드하면 \x64\Release 아래 TeamBest_SSD_Shell.exe 가 생성된다. cmd 창에 TeamBest_SSD_Shell.exe shell_script.txt 라고 입력하면 shell_script.txt 에 들어있는 tc 이름을 찾아 해당 tc 가 순차적으로 수행된다.
  ![image](https://github.com/user-attachments/assets/c57f8efb-1f16-487b-9bec-6dfcd0cce2b7)
