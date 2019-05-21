readonly PROJECT=$(realpath $(dirname $BASH_SOURCE))

readonly SRCDIR=$PROJECT/src

readonly OUTDIR=$PROJECT/out

readonly LOGFILE=$OUTDIR/build.log

readonly API_LEVEL=21

readonly ARM_TRIPLE_API=armv7a-linux-androideabi$API_LEVEL
readonly X86_TRIPLE_API=i686-linux-android$API_LEVEL

readonly ARM_TRIPLE=arm-linux-androideabi
readonly X86_TRIPLE=i686-linux-android

readonly ARM_COMPILER=$ANDROID_NDK_HOME/toolchains/llvm/prebuilt/linux-x86_64/bin/$ARM_TRIPLE_API-clang++
readonly X86_COMPILER=$ANDROID_NDK_HOME/toolchains/llvm/prebuilt/linux-x86_64/bin/$X86_TRIPLE_API-clang++

readonly ARM_ARCHIVER=$ANDROID_NDK_HOME/toolchains/llvm/prebuilt/linux-x86_64/bin/$ARM_TRIPLE-ar
readonly X86_ARCHIVER=$ANDROID_NDK_HOME/toolchains/llvm/prebuilt/linux-x86_64/bin/$X86_TRIPLE-ar

mkdir $OUTDIR 2>/dev/null
exec 3>&2 2>$LOGFILE

function abort {
	echo >&3 -e "\e[01;33mERROR\e[0m: \e[01;31m$1\e[0m"
	exit 1
}

function compile {
	local COMPILER=$1
	shift
	if ! $COMPILER $@; then
		abort "Compilation failed"
	fi
}

function archive {
	local ARCHIVER=$1
	shift
	if ! $ARCHIVER $@; then
		abort "Archiving failed"
	fi
}

function build_for {
	rm -f ./libsocket++_$1.a *.o

	compile $2 -x c -c -O2 -fomit-frame-pointer $SRCDIR/libunixsocket.c -o ./libunixsocket.c.o
	for SOURCE in $SRCDIR/*.cpp; do
		compile $2 -c -O2 -fno-rtti -fomit-frame-pointer $SOURCE -I$SRCDIR/include -o ./${SOURCE##*/}.o
	done

	archive $3 -rcs ./libsocket++_$1.a ./*.o
	rm -f ./*.o
}

if ! cd $OUTDIR; then
	abort "Could not change working directory to $OUTDIR"
fi

build_for arm $ARM_COMPILER $ARM_ARCHIVER
build_for x86 $X86_COMPILER $X86_ARCHIVER
