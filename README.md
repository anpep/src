[![lint](https://github.com/anpep/unix/actions/workflows/lint.yaml/badge.svg)](https://github.com/anpep/unix/actions/workflows/lint.yaml)
[![build](https://github.com/anpep/unix/actions/workflows/build.yaml/badge.svg)](https://github.com/anpep/unix/actions/workflows/build.yaml)
[![test](https://github.com/anpep/unix/actions/workflows/test.yaml/badge.svg)](https://github.com/anpep/unix/actions/workflows/test.yaml)

About
=====
This repository contains the source code for a long-term experiment on OS
development.

A previous attempt has been made [here](https://github.com/anpep/osdev), and in
a myriad other places that are either not publicly available, or have been lost
to the annals of time.

Aim of this project
-------------------

Programming is fun! Don't ever let [Hacker News](https://news.ycombinator.com)
trolls take that from us (:

I'm really interested on the learning outcomes of designing and implementing a
basic operating system. I aim at POSIX compliance for portability reasons,
while stating nothing about the soundness or the validity of POSIX.

What's the name of this project?
--------------------------------

I'm strongly opposed to giving special names to things that are unoriginal.

Building
========

Just run `make` to build the system and run unit tests. It does not do much for
now, nor does it support any architecture-/platform-specific code (I'm still
working on the C library). It will in the future!

Build dependencies
------------------

`clang` and `gcc` are tested and supported. You also need `clang-format` and
`clang-tidy` for linting.

Contributing
============

Pull requests are welcome, just make sure you run `make lint` before commiting
your changes.

License
=======
Copyright (C) 2023 Ángel Pérez \<ap@anpep.co\>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
