# Study list_sort algorithm with merge sort implementation in kernel
> Here shows the alorithm mechanism with a demo, it should be easier for beginners to understand.

> Please ignore the coding style in this .c file, just put all related thing in a single for reader easy to find related data structure and help functions.

## Play with Makefile
### Build
    make

### Run
    ./test_list_sort

### Clean up
    make clean 


## Play with Makefile
### Build
    meson setup build
    ninja -C build

### Run
    .build/test_list_sort

### Clean up
    rm -rf build


## Example of demo output
> Play with your terimal, you will see color output which would be more readable.

    #################################
    ########### Initial #############
      0 -> N
      1 -> W
      2 -> L
      3 -> R
      4 -> B
      5 -> B
      6 -> M
      7 -> Q
      8 -> B
      9 -> H
     10 -> C
     11 -> D
     12 -> A
     13 -> R
     14 -> Z
    pending lists start-----cnt:01-----
        sublist@0xf27d5740 start-----cnt:01-----
              0 -> N
        sublist@0xf27d5740  end*****cnt:01*****
    pending lists  end*****cnt:01*****
    remaing elements start-----cnt:01-----
              1 -> W
              2 -> L
              3 -> R
              4 -> B
              5 -> B
              6 -> M
              7 -> Q
              8 -> B
              9 -> H
             10 -> C
             11 -> D
             12 -> A
             13 -> R
             14 -> Z
             15 -> O
    remaing elements  end*****cnt:01*****
    pending lists start-----cnt:02-----
        sublist@0xf27d5760 start-----cnt:02-----
              1 -> W
        sublist@0xf27d5760  end*****cnt:02*****
        sublist@0xf27d5740 start-----cnt:02-----
              0 -> N
        sublist@0xf27d5740  end*****cnt:02*****
    pending lists  end*****cnt:02*****
    remaing elements start-----cnt:02-----
              2 -> L
              3 -> R
              4 -> B
              5 -> B
              6 -> M
              7 -> Q
              8 -> B
              9 -> H
             10 -> C
             11 -> D
             12 -> A
             13 -> R
             14 -> Z
             15 -> O
    remaing elements  end*****cnt:02*****
    pending lists start-----cnt:03-----
        sublist@0xf27d5780 start-----cnt:03-----
              2 -> L
        sublist@0xf27d5780  end*****cnt:03*****
        sublist@0xf27d5740 start-----cnt:03-----
              0 -> N
              1 -> W
        sublist@0xf27d5740  end*****cnt:03*****
    pending lists  end*****cnt:03*****
    remaing elements start-----cnt:03-----
              3 -> R
              4 -> B
              5 -> B
              6 -> M
              7 -> Q
              8 -> B
              9 -> H
             10 -> C
             11 -> D
             12 -> A
             13 -> R
             14 -> Z
             15 -> O
    remaing elements  end*****cnt:03*****
    pending lists start-----cnt:04-----
        sublist@0xf27d57a0 start-----cnt:04-----
              3 -> R
        sublist@0xf27d57a0  end*****cnt:04*****
        sublist@0xf27d5780 start-----cnt:04-----
              2 -> L
        sublist@0xf27d5780  end*****cnt:04*****
        sublist@0xf27d5740 start-----cnt:04-----
              0 -> N
              1 -> W
        sublist@0xf27d5740  end*****cnt:04*****
    pending lists  end*****cnt:04*****
    remaing elements start-----cnt:04-----
              4 -> B
              5 -> B
              6 -> M
              7 -> Q
              8 -> B
              9 -> H
             10 -> C
             11 -> D
             12 -> A
             13 -> R
             14 -> Z
             15 -> O
    remaing elements  end*****cnt:04*****
    pending lists start-----cnt:05-----
        sublist@0xf27d57c0 start-----cnt:05-----
              4 -> B
        sublist@0xf27d57c0  end*****cnt:05*****
        sublist@0xf27d5780 start-----cnt:05-----
              2 -> L
              3 -> R
        sublist@0xf27d5780  end*****cnt:05*****
        sublist@0xf27d5740 start-----cnt:05-----
              0 -> N
              1 -> W
        sublist@0xf27d5740  end*****cnt:05*****
    pending lists  end*****cnt:05*****
    remaing elements start-----cnt:05-----
              5 -> B
              6 -> M
              7 -> Q
              8 -> B
              9 -> H
             10 -> C
             11 -> D
             12 -> A
             13 -> R
             14 -> Z
             15 -> O
    remaing elements  end*****cnt:05*****
    pending lists start-----cnt:06-----
        sublist@0xf27d57e0 start-----cnt:06-----
              5 -> B
        sublist@0xf27d57e0  end*****cnt:06*****
        sublist@0xf27d57c0 start-----cnt:06-----
              4 -> B
        sublist@0xf27d57c0  end*****cnt:06*****
        sublist@0xf27d5780 start-----cnt:06-----
              2 -> L
              0 -> N
              3 -> R
              1 -> W
        sublist@0xf27d5780  end*****cnt:06*****
    pending lists  end*****cnt:06*****
    remaing elements start-----cnt:06-----
              6 -> M
              7 -> Q
              8 -> B
              9 -> H
             10 -> C
             11 -> D
             12 -> A
             13 -> R
             14 -> Z
             15 -> O
    remaing elements  end*****cnt:06*****
    pending lists start-----cnt:07-----
        sublist@0xf27d5800 start-----cnt:07-----
              6 -> M
        sublist@0xf27d5800  end*****cnt:07*****
        sublist@0xf27d57c0 start-----cnt:07-----
              4 -> B
              5 -> B
        sublist@0xf27d57c0  end*****cnt:07*****
        sublist@0xf27d5780 start-----cnt:07-----
              2 -> L
              0 -> N
              3 -> R
              1 -> W
        sublist@0xf27d5780  end*****cnt:07*****
    pending lists  end*****cnt:07*****
    remaing elements start-----cnt:07-----
              7 -> Q
              8 -> B
              9 -> H
             10 -> C
             11 -> D
             12 -> A
             13 -> R
             14 -> Z
             15 -> O
    remaing elements  end*****cnt:07*****
    pending lists start-----cnt:08-----
        sublist@0xf27d5820 start-----cnt:08-----
              7 -> Q
        sublist@0xf27d5820  end*****cnt:08*****
        sublist@0xf27d5800 start-----cnt:08-----
              6 -> M
        sublist@0xf27d5800  end*****cnt:08*****
        sublist@0xf27d57c0 start-----cnt:08-----
              4 -> B
              5 -> B
        sublist@0xf27d57c0  end*****cnt:08*****
        sublist@0xf27d5780 start-----cnt:08-----
              2 -> L
              0 -> N
              3 -> R
              1 -> W
        sublist@0xf27d5780  end*****cnt:08*****
    pending lists  end*****cnt:08*****
    remaing elements start-----cnt:08-----
              8 -> B
              9 -> H
             10 -> C
             11 -> D
             12 -> A
             13 -> R
             14 -> Z
             15 -> O
    remaing elements  end*****cnt:08*****
    pending lists start-----cnt:09-----
        sublist@0xf27d5840 start-----cnt:09-----
              8 -> B
        sublist@0xf27d5840  end*****cnt:09*****
        sublist@0xf27d5800 start-----cnt:09-----
              6 -> M
              7 -> Q
        sublist@0xf27d5800  end*****cnt:09*****
        sublist@0xf27d57c0 start-----cnt:09-----
              4 -> B
              5 -> B
        sublist@0xf27d57c0  end*****cnt:09*****
        sublist@0xf27d5780 start-----cnt:09-----
              2 -> L
              0 -> N
              3 -> R
              1 -> W
        sublist@0xf27d5780  end*****cnt:09*****
    pending lists  end*****cnt:09*****
    remaing elements start-----cnt:09-----
              9 -> H
             10 -> C
             11 -> D
             12 -> A
             13 -> R
             14 -> Z
             15 -> O
    remaing elements  end*****cnt:09*****
    pending lists start-----cnt:0a-----
        sublist@0xf27d5860 start-----cnt:0a-----
              9 -> H
        sublist@0xf27d5860  end*****cnt:0a*****
        sublist@0xf27d5840 start-----cnt:0a-----
              8 -> B
        sublist@0xf27d5840  end*****cnt:0a*****
        sublist@0xf27d57c0 start-----cnt:0a-----
              4 -> B
              5 -> B
              6 -> M
              7 -> Q
        sublist@0xf27d57c0  end*****cnt:0a*****
        sublist@0xf27d5780 start-----cnt:0a-----
              2 -> L
              0 -> N
              3 -> R
              1 -> W
        sublist@0xf27d5780  end*****cnt:0a*****
    pending lists  end*****cnt:0a*****
    remaing elements start-----cnt:0a-----
             10 -> C
             11 -> D
             12 -> A
             13 -> R
             14 -> Z
             15 -> O
    remaing elements  end*****cnt:0a*****
    pending lists start-----cnt:0b-----
        sublist@0xf27d5880 start-----cnt:0b-----
             10 -> C
        sublist@0xf27d5880  end*****cnt:0b*****
        sublist@0xf27d5840 start-----cnt:0b-----
              8 -> B
              9 -> H
        sublist@0xf27d5840  end*****cnt:0b*****
        sublist@0xf27d57c0 start-----cnt:0b-----
              4 -> B
              5 -> B
              6 -> M
              7 -> Q
        sublist@0xf27d57c0  end*****cnt:0b*****
        sublist@0xf27d5780 start-----cnt:0b-----
              2 -> L
              0 -> N
              3 -> R
              1 -> W
        sublist@0xf27d5780  end*****cnt:0b*****
    pending lists  end*****cnt:0b*****
    remaing elements start-----cnt:0b-----
             11 -> D
             12 -> A
             13 -> R
             14 -> Z
             15 -> O
    remaing elements  end*****cnt:0b*****
    pending lists start-----cnt:0c-----
        sublist@0xf27d58a0 start-----cnt:0c-----
             11 -> D
        sublist@0xf27d58a0  end*****cnt:0c*****
        sublist@0xf27d5880 start-----cnt:0c-----
             10 -> C
        sublist@0xf27d5880  end*****cnt:0c*****
        sublist@0xf27d5840 start-----cnt:0c-----
              8 -> B
              9 -> H
        sublist@0xf27d5840  end*****cnt:0c*****
        sublist@0xf27d57c0 start-----cnt:0c-----
              4 -> B
              5 -> B
              2 -> L
              6 -> M
              0 -> N
              7 -> Q
              3 -> R
              1 -> W
        sublist@0xf27d57c0  end*****cnt:0c*****
    pending lists  end*****cnt:0c*****
    remaing elements start-----cnt:0c-----
             12 -> A
             13 -> R
             14 -> Z
             15 -> O
    remaing elements  end*****cnt:0c*****
    pending lists start-----cnt:0d-----
        sublist@0xf27d58c0 start-----cnt:0d-----
             12 -> A
        sublist@0xf27d58c0  end*****cnt:0d*****
        sublist@0xf27d5880 start-----cnt:0d-----
             10 -> C
             11 -> D
        sublist@0xf27d5880  end*****cnt:0d*****
        sublist@0xf27d5840 start-----cnt:0d-----
              8 -> B
              9 -> H
        sublist@0xf27d5840  end*****cnt:0d*****
        sublist@0xf27d57c0 start-----cnt:0d-----
              4 -> B
              5 -> B
              2 -> L
              6 -> M
              0 -> N
              7 -> Q
              3 -> R
              1 -> W
        sublist@0xf27d57c0  end*****cnt:0d*****
    pending lists  end*****cnt:0d*****
    remaing elements start-----cnt:0d-----
             13 -> R
             14 -> Z
             15 -> O
    remaing elements  end*****cnt:0d*****
    pending lists start-----cnt:0e-----
        sublist@0xf27d58e0 start-----cnt:0e-----
             13 -> R
        sublist@0xf27d58e0  end*****cnt:0e*****
        sublist@0xf27d58c0 start-----cnt:0e-----
             12 -> A
        sublist@0xf27d58c0  end*****cnt:0e*****
        sublist@0xf27d5840 start-----cnt:0e-----
              8 -> B
             10 -> C
             11 -> D
              9 -> H
        sublist@0xf27d5840  end*****cnt:0e*****
        sublist@0xf27d57c0 start-----cnt:0e-----
              4 -> B
              5 -> B
              2 -> L
              6 -> M
              0 -> N
              7 -> Q
              3 -> R
              1 -> W
        sublist@0xf27d57c0  end*****cnt:0e*****
    pending lists  end*****cnt:0e*****
    remaing elements start-----cnt:0e-----
             14 -> Z
             15 -> O
    remaing elements  end*****cnt:0e*****
    pending lists start-----cnt:0f-----
        sublist@0xf27d5900 start-----cnt:0f-----
             14 -> Z
        sublist@0xf27d5900  end*****cnt:0f*****
        sublist@0xf27d58c0 start-----cnt:0f-----
             12 -> A
             13 -> R
        sublist@0xf27d58c0  end*****cnt:0f*****
        sublist@0xf27d5840 start-----cnt:0f-----
              8 -> B
             10 -> C
             11 -> D
              9 -> H
        sublist@0xf27d5840  end*****cnt:0f*****
        sublist@0xf27d57c0 start-----cnt:0f-----
              4 -> B
              5 -> B
              2 -> L
              6 -> M
              0 -> N
              7 -> Q
              3 -> R
              1 -> W
        sublist@0xf27d57c0  end*****cnt:0f*****
    pending lists  end*****cnt:0f*****
    remaing elements start-----cnt:0f-----
             15 -> O
    remaing elements  end*****cnt:0f*****
    pending lists start-----cnt:10-----
        sublist@0xf27d5920 start-----cnt:10-----
             15 -> O
        sublist@0xf27d5920  end*****cnt:10*****
        sublist@0xf27d5900 start-----cnt:10-----
             14 -> Z
        sublist@0xf27d5900  end*****cnt:10*****
        sublist@0xf27d58c0 start-----cnt:10-----
             12 -> A
             13 -> R
        sublist@0xf27d58c0  end*****cnt:10*****
        sublist@0xf27d5840 start-----cnt:10-----
              8 -> B
             10 -> C
             11 -> D
              9 -> H
        sublist@0xf27d5840  end*****cnt:10*****
        sublist@0xf27d57c0 start-----cnt:10-----
              4 -> B
              5 -> B
              2 -> L
              6 -> M
              0 -> N
              7 -> Q
              3 -> R
              1 -> W
        sublist@0xf27d57c0  end*****cnt:10*****
    pending lists  end*****cnt:10*****
    remaing elements start-----cnt:10-----
    remaing elements  end*****cnt:10*****
    pending lists start-----cnt:ff-----
        sublist@0xf27d5900 start-----cnt:ff-----
             14 -> Z
        sublist@0xf27d5900  end*****cnt:ff*****
        sublist@0xf27d58c0 start-----cnt:ff-----
             12 -> A
             13 -> R
        sublist@0xf27d58c0  end*****cnt:ff*****
        sublist@0xf27d5840 start-----cnt:ff-----
              8 -> B
             10 -> C
             11 -> D
              9 -> H
        sublist@0xf27d5840  end*****cnt:ff*****
        sublist@0xf27d57c0 start-----cnt:ff-----
              4 -> B
              5 -> B
              2 -> L
              6 -> M
              0 -> N
              7 -> Q
              3 -> R
              1 -> W
        sublist@0xf27d57c0  end*****cnt:ff*****
    pending lists  end*****cnt:ff*****
    generate result start-----cnt:ff-----
             15 -> O
    generate result  end*****cnt:ff*****
    pending lists start-----cnt:fe-----
        sublist@0xf27d58c0 start-----cnt:fe-----
             12 -> A
             13 -> R
        sublist@0xf27d58c0  end*****cnt:fe*****
        sublist@0xf27d5840 start-----cnt:fe-----
              8 -> B
             10 -> C
             11 -> D
              9 -> H
        sublist@0xf27d5840  end*****cnt:fe*****
        sublist@0xf27d57c0 start-----cnt:fe-----
              4 -> B
              5 -> B
              2 -> L
              6 -> M
              0 -> N
              7 -> Q
              3 -> R
              1 -> W
        sublist@0xf27d57c0  end*****cnt:fe*****
    pending lists  end*****cnt:fe*****
    generate result start-----cnt:fe-----
             15 -> O
             14 -> Z
    generate result  end*****cnt:fe*****
    pending lists start-----cnt:fd-----
        sublist@0xf27d5840 start-----cnt:fd-----
              8 -> B
             10 -> C
             11 -> D
              9 -> H
        sublist@0xf27d5840  end*****cnt:fd*****
        sublist@0xf27d57c0 start-----cnt:fd-----
              4 -> B
              5 -> B
              2 -> L
              6 -> M
              0 -> N
              7 -> Q
              3 -> R
              1 -> W
        sublist@0xf27d57c0  end*****cnt:fd*****
    pending lists  end*****cnt:fd*****
    generate result start-----cnt:fd-----
             12 -> A
             15 -> O
             13 -> R
             14 -> Z
    generate result  end*****cnt:fd*****
    pending lists start-----cnt:fc-----
        sublist@0xf27d57c0 start-----cnt:fc-----
              4 -> B
              5 -> B
              2 -> L
              6 -> M
              0 -> N
              7 -> Q
              3 -> R
              1 -> W
        sublist@0xf27d57c0  end*****cnt:fc*****
    pending lists  end*****cnt:fc*****
    generate result start-----cnt:fc-----
             12 -> A
              8 -> B
             10 -> C
             11 -> D
              9 -> H
             15 -> O
             13 -> R
             14 -> Z
    generate result  end*****cnt:fc*****
    #################################
    ########### After Sort ##########
     12 -> A
      4 -> B
      5 -> B
      8 -> B
     10 -> C
     11 -> D
      9 -> H
      2 -> L
      6 -> M
      0 -> N
     15 -> O
      7 -> Q
      3 -> R
     13 -> R
      1 -> W
    
