REM Automatically generated from Makefile
mkdir -p obj
..\..\gbdk\bin\lcc -Wf--peep-asm -Wf--peep-return -Wf--opt-code-speed -Wf"--max-allocs-per-node 1000000" -Wa-l -Wl-m -Wl-j -Wf-MMD -c -o obj\console.o GBIntro\console.c
..\..\gbdk\bin\lcc -Wf--peep-asm -Wf--peep-return -Wf--opt-code-speed -Wf"--max-allocs-per-node 1000000" -Wa-l -Wl-m -Wl-j -Wf-MMD -c -o obj\main.o GBIntro\main.c
..\..\gbdk\bin\lcc -Wf--peep-asm -Wf--peep-return -Wf--opt-code-speed -Wf"--max-allocs-per-node 1000000" -Wa-l -Wl-m -Wl-j -o obj\GBIntro.gb obj\console.o obj\main.o obj\console.o obj\main.o 
