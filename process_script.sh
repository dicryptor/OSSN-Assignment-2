#!/bin/bash
child_pid=0
showAllProcess() {
    ps -ef
}

installProgram() {
    echo -e "\nEnter name of program to install"
    read progname_ins
    sudo apt-get install ${progname_ins}
}

fpfunc(){
    while true
    do
        echo "Hello world from forked process"
        sleep 1
    done
}

fork(){
    fpfunc &
    child_pid=$!
    echo "Child process started with PID of $child_pid"
}

killChild(){
    # child_pid=$!
    # echo ${child_pid}
    echo "Terminating child process $child_pid"
    kill -9 $child_pid
}

rebootSystem() {
    echo "Rebooting the system now"
    sleep 1
    sudo shutdown -r now
}

while :
do
    clear
    cat<<EOF
    ==============================
            Program Menu
    ------------------------------
    Please enter your choice:

    Show All Running Process    (1)
    Install a Program           (2)
    Generate Child Process      (3)
    Terminate Child Process     (4)
    Reboot the System           (5)
    Exit Program                (Q)uit
    ------------------------------
EOF
    read -n1 -s
    case "$REPLY" in
    "1")  showAllProcess ;;
    "2")  installProgram ;;
    "3")  fork ;;
    "4")  killChild ;;
    "5")  rebootSystem ;;
    "Q")  exit                      ;;
    "q")  echo "case sensitive!!"   ;;
     * )  echo "invalid option"     ;;
    esac
    sleep 1
done

