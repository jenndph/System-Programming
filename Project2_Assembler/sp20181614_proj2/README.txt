#README
#20181614 김주연

컴파일/ 실행 방법
# make
# ./20181614.out

Usage
sicsim> h[elp]
: Shell에서 실행 가능한 모든 명령어들의 리스트를 화면에 출력해준다.

sicsim> d[ir]
: 현재 디렉터리에 있는 파일들을 출력한다.

sicsim> q[uit]
: sicsim을 종료한다.

sicsim> hi[story]
: 현재까지 사용한 명령어들을 순서대로 번호와 함께 보여준다. 
  가장 최근에 사용한 명령어가 리스트의 하단에 오도록 한다.

sicsim> du[mp] [start, end]
: 할당되어 있는 메모리의 내용을 출력한다.

sicsim> e[dit] address, value
: 메모리의 address 번지의 값을 value에 지정된 값으로 변경한다.

sicsim> f[ill] start, end, value
: 메모리의 start 번지부터 end 번지까지의 값을 value에 지정된 값으로 변경한다.

sicsim> reset
: 메모리 전체를 전부 0 으로 변경한다.

sicsim> opcode mnemonic
: 명령어를 입력하면 해당하는 opcode를 출력한다.

sicsim> opcodelist
: opcode Hash Table의 내용을 출력한다.

sicsim> type filename
: filename에 해당하는 파일을 현재 디렉터리에서 읽어서 화면에 출력한다.

sicsim> assemble filename
: filename에 해당하는 소스 파일을 읽어서 object 파일과 리스팅 파일을 만든다.

sicsim> symbol
: assemble 과정 중에 생성된 symbol table을 화면에 출력한다.
