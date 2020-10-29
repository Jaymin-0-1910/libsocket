# libsocket

## Notes

Following are building instructions for Android OS API 21 on arm and x86 architectures. If you want to build
for any other OS or architecture, you may freely adapt the whole code given conditions in the license are met.

## Requirements

1. Windows machine with PowerShell
2. Fairly modern Android NDK

## Building

1. Set `$Env:AndroidNDKHome` to NDK path
2. Run `build.ps1` script

Library archives will be placed in `out/` folder along with entire terminal output in `out/build.log`. You
need to check build.log first for any kind of debugging.

## License

See LICENSE file for license information.
