# kernel-modules

Kernel modules developmement stuff.
It's based on [http://www.tldp.org/LDP/lkmpg/2.6/html/lkmpg.html](http://www.tldp.org/LDP/lkmpg/2.6/html/lkmpg.html).

## Commands

A quick description and use examples of useful commands.

### modinfo

_modinfo_ shows informations about a module.

> Note that kernel 2.6 introduces a new file naming convention: 
> kernel modules now have a .ko extension (in place of the old .o extension) 
> which easily distinguishes them from conventional object files. 
> The reason for this is that they contain an additional .modinfo section
> that's where additional information about the module is kept. 

#### Usage

```
$ modinfo hello-4.ko
filename:       /home/mscomparin/kernel-modules/hello-4.ko
description:    A sample driver
author:         Mauro Scomparin <scompo@gmail.com>
license:        GPL
srcversion:     CB6A81B2EACBE20AE4800CF
depends:        
vermagic:       4.8.0-28-generic SMP mod_unload modversions 
```

### insmod

_insmod_ installs the specified module in the system.
This command should be run as root.

#### Usage

```
# insmod hello-4.ko  
```

### rmmod

_rmmod_ removes the specified module from the system.
This command should be run as root.

#### Usage

```
# rmmod hello-4.ko
```
