# Doggle - A Boggle simulator for DOS

For those who don't know what Boggle is, see the Wikipedia page: https://en.wikipedia.org/wiki/Boggle

Doggle is an all-in-one board generator, timer, and dictionary for Boggle, designed as a DOS program. It is intentionally designed to be very tiny, and can run on a system with only 128 KiB of RAM and an 8088 CPU. It is written entirely in C and can be cross-compiled for DOS on a modern system using Open Watcom 2.0. It can be used on real hardware with DOS, or you can run it in DOSBox if you'd prefer.

I wrote this as a C coding exercise in preparation for making an entirely different program. I also wanted a virtual Boggle board and figured this would be a good way to gain experience while making it. :P

## Compilation

Assuming you have a working installation of Open Watcom 2.0, all you need to do is:

    wcl -mt -0 doggle.c

This should almost instantly spit out a `doggle.com` file, which is the finished program.

## Running

If you want to run the program on the same machine you just compiled it on, just run:

    dosbox doggle.com

Obviously this requires that DOSBox is installed on your system. The corresponding dictionary file will be accessible within the emulator.

If you intend to run Doggle inside a virtual machine of some sort, or you want to run it on real hardware, you need to copy both the `doggle.com` and the `dict.txt` files to a (virtual) floppy disk. Insert the disk into your machine, change to the drive and directory containing the Doggle executable, and then run `doggle` on the machine. How exactly you do this will vary depending on your equipment, software, and setup.

If the `dict.txt` file is missing, the dictionary feature will be disabled. The rest of Doggle will work however.

## System compatibility

Hardware-wise, Doggle can run on an original IBM PC with 128 KB RAM. However, for reasons I do not know, the speaker doesn't work and so no alarm tone is played at the end of a Boggle round. An IBM PC AT with 256 KB RAM works flawlessly, including sound. Sound also works in DOSBox.

As far as required DOS version, DOSBox, FreeDOS 1.3, and IBM PC-DOS 3.30 all work (including sound). PC-DOS 1.10 sadly does not work, and will hang indefinitely when you try to run Doggle.

I used the DOSBox and 86Box emulators for testing. The code is fairly straightforward, so other emulators are likely to work too, though the sound may or may not work.

## License and Copyright

For doggle.c and README.md:  
Copyright (c) 2023 Aaron Rainbolt. Licensed under the [MIT License](https://opensource.org/license/mit/).

For dict.txt:  
  dict.txt has been extracted from GNU Aspell and adapted to be smaller and work better as a Boggle dictionary.  
  Copyright (c) 2002-2019 Kevin Atkinson and the GNU Aspell contributors. Copyright (c) 2023 Aaron Rainbolt. Licensed under the [GNU Lesser General Public License v2.1](https://www.gnu.org/licenses/old-licenses/lgpl-2.1.en.html).

Doggle is in no way affiliated with Hasbro, the owners of the Boggle trademark.
