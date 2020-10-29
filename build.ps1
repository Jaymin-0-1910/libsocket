$RootDir = "$PSScriptRoot"
$SrcDir = "$RootDir\src"
$OutDir = "$RootDir\out"

$LogFile = "$OutDir\build.log"

$NDKBinDir = "$Env:AndroidNDKHome\toolchains\llvm\prebuilt\windows-x86_64\bin"

$APILevel = 21

$ARMCompiler = "$NDKBinDir\armv7a-linux-androideabi$APILevel-clang++"
$X86Compiler = "$NDKBinDir\i686-linux-android$APILevel-clang++"
$ARMArchiver = "$NDKBinDir\arm-linux-androideabi-ar"
$X86Archiver = "$NDKBinDir\i686-linux-android-ar"

New-Item -Force -ItemType Directory -Path "$OutDir" >$Null
Set-Location -Path "$OutDir" *>"$LogFile"

Function Build {
	param(
		$Arch,
		$Compiler,
		$Archiver
	)

	Get-ChildItem -File -Name -Include @("*.c", "*.cpp") -Path "$SrcDir" | ForEach-Object {
		if ("$_" -like "*.c") {
			cmd /C "$Compiler" -x c -c -O2 -fomit-frame-pointer "$SrcDir\$_" -o ".\$_.o"
		} else {
			cmd /C "$Compiler" -c -O2 -fno-rtti -fomit-frame-pointer "$SrcDir\$_" -o ".\$_.o"
		}
	}
	cmd /C "$Archiver" -rcs ".\libsocket_$Arch.a" @(Get-ChildItem -File -Name -Include ".\*.o") #NOTE: I really don't like the way @(...) is used here, I'd rather want to use wildcard here as well. This line really pisses me off.

	Remove-Item -Force -Include "*.o" -Path ".\*"
}

Build -Arch "arm" -Compiler "$ARMCompiler" -Archiver "$ARMArchiver" *>>"$LogFile"
Build -Arch "x86" -Compiler "$X86Compiler" -Archiver "$X86Archiver" *>>"$LogFile"
