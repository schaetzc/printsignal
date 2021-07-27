# printsignal

`printsignal` is a Linux command line tool.
It pauses until it receives a signal,
then prints the signal together with the PID and UID of the sender (if available),
and then exits.

All signals are caught, except SIGKILL and SIGSTOP.

The command `strace -o '| { grep -m1 "^--- "; pkill -CONT -P $PPID sleep; pkill -P $PPID sleep; }' sleep inf` essentially does the same, but does not print signals sent directly to `strace`, for instance when pressing <kbd>Ctrl</kbd><kbd>C</kbd> or <kbd>Ctrl</kbd><kbd>Z</kbd> on the terminal. However, `strace` also catches SIGSTOP which `printsignal` cannot do.

## Compile

Download this repository, then run `make`:

    git clone "https://github.com/schaetzc/printsignal.git"
    cd printsignal
    make

## Run

Interactive examples. `$` is the prompt.

    $ ./printsignal # press enter, then ctrl+c
    This is process 2345 waiting for a signal
    Caught signal 2 (Interrupt) from kernel

    $ ./printsignal # press enter, then ctrl+z
    This is process 2346 waiting for a signal
    Caught signal 20 (Stopped) from kernel

    $ ./printsignal & pid=$!; sleep 1; kill "$pid"
    This is process 2347 waiting for a signal
    Caught signal 15 (Terminated) from process 1234 of user 1000

    $ ./printsignal & pid=$!; sleep 1; kill -SIGRTMIN "$pid"
    This is process 2349 waiting for a signal
    Caught signal 34 (Real-time signal 0) from process 1234 of user 1000    

    $ timeout 1 ./printsignal
    This is process 2352 waiting for a signal
    Caught signal 15 (Terminated) from process 3351 of user 1000
