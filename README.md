### Sample UWP project for JXcore

This repository has everything included in order to show basic usage of JXcore under a UWP application.

!You will need VS 2015+ to use the solution file. 

#### Compiling JXcore for UWP

This project contains JXcore 0.3.1.2 binaries pre-built under `App1/jxcore` folder.

In case you want to build jxcore for UWP; 

(script below will generate JXcore UWP Visual Studio project)

```
> git clone https://github.com/jxcore/jxcore

> vcbuild.bat --win-onecore --shared-library --engine-chakra x64
```

!replace x64 to `arm` or `ia32` for other targets

#### LICENSE

MIT