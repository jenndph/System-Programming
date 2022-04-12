# 시스템프로그래밍 (CSE4100)


## Project1. SIC/XE Shell
SIC/XE 머신을 구현하기 위한 전 단계
* Assembler, Linker, Loader들을 실행하게 될 셸(shell) 구현
* 컴파일을 통해서 만들어진 object code가 적재되고 실행될 메모리 공간 구현
* mnemonic (ADD, COMP, FLOAT, etc ...)을 opcode 값으로 변환하는 OPCODE 테이블과 관련 명령어들을 구현


## Project2. Assembler
Simple SIC Assembler 구현
* Assembly language로 작성된 source program을 equivalent machine object code로 변환
* Assemble 과정 중 생성된 symbol table과 결과물인 object 파일을 볼 수 있는 기능을 제공
* Assembler 기능
  * Pass 1
    * *.asm 파일을 읽어 와서 instruction format, 변수/상수의 메모리 크기를 계산해 location 값을 구함
    * symbol이 존재한다면 그것을 linked list 형태로 저장하여 이후 pass 2에서 사용할 수 있도록 함
  * Pass 2
    * pass 1에서 생성한 임시 lst 파일을 한 줄 씩 읽어 와서 해당 줄의 object code를 계산 
    * 계산한 object code를 이용하여 obj 파일을 생성


## Project 3. Linking Loader
Linking loader 기능 추가
* Assemble 과정을 통해 생성된 object 파일을 1M의 가상 메모리에 loading하는 기능을 구현
* 이 때 여러 object 파일이 인풋으로 들어온 경우 loading 전에 linking을 수행
* Linking loader 기능
  * Pass 1
    * Header record에서는 control section, Define record에서는 defined external symbol의 이름과 주소를 읽어 와서 CSADDR값을 고려한 실제 주소 값으로 ESTAB(external symbol table)에 저장
    * 모든 object 파일에 대해 해당 기능을 수행하고 로드맵(ESTAB)을 출력
  * Pass 2
    * Refer record의 reference number와 변수 이름을 array 형식으로 저장
    * Text record의 내용을 EXECADDR가 가리키는 가상 메모리 주소에 저장 
    * pass 1에서 생성한 ESTAB의 내용을 이용하여 R record의 변수들에 대한 relocation 작업을 수행 

