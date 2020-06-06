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

sicsim> progaddr [address]
: 프로그램이 시작될 주소를 설정한다

sicsim> loader [object filename1] [object filename2] [...]
: filename1, ...에 해당하는 object 파일을 읽어서 Linking 작업을 수행한 후 
  가상 메모리에 결과를 기록한다.

sicsim> bp [address]
: sicsim에 breakpoint를 지정한다.

sicsim> bp clear
: sicsim에 존재하는 breakpoint를 전부 삭제한다.

sicsim> bp
: sicsim에 존재하는 breakpoint를 전부 화면에 출력한다.

sicsim> run
: loader 명령어의 수행으로 메모리에 load된 프로그램을 실행한다.
