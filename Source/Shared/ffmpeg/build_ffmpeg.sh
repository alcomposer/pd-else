#!/bin/bash

# Variables
FFMPEG_DIR="ffmpeg-7.0.1"
OS=$(uname)


# Define platform-specific configurations
if [[ "$OS" == "Darwin" ]]; then
    ffmpeg_config="--extra-cflags=-mmacosx-version-min=10.9 --extra-ldflags=-mmacosx-version-min=10.9"
    ffmpeg_cc="clang -arch x86_64 -arch arm64"
    sed -i.bak '5021c\
            _DEPCMD='\''$(DEP$(1)) $(DEP$(1)FLAGS) $($(1)DEP_FLAGS) $< 2>&1 | grep "^Note:.*file:" | sed -e "s^.*file: *^$@: ^" | tr \\\\\\\\/ \/ > $(@:.o=.d)'\'' '$'\n' "$FFMPEG_DIR/configure"
elif [[ "$OS" == "Linux" ]]; then
    ffmpeg_config="--enable-pic"
    ffmpeg_cc="${CC:-gcc}"
elif [ "$(expr substr $(uname -s) 1 10)" == "MINGW64_NT" ]; then
    ffmpeg_config="--toolchain=msvc"
    ffmpeg_cc="${CC}"
    sed -i.bak '5021c\
            _DEPCMD='\''$(DEP$(1)) $(DEP$(1)FLAGS) $($(1)DEP_FLAGS) $< 2>&1 | grep "^Note:.*file:" | sed -e "s^.*file: *^$@: ^" | tr \\\\\\\\/ \/ > $(@:.o=.d)'\'' '$'\n' "$FFMPEG_DIR/configure"
else
    echo "Unsupported OS: $OS"
    exit 1
fi

# Configure and compile FFmpeg
cd "$FFMPEG_DIR"
./configure --disable-asm --disable-libdrm --disable-vaapi --enable-static --disable-shared --enable-optimizations --disable-debug --disable-doc \
            --disable-programs --disable-iconv --disable-avdevice --disable-postproc --disable-network \
            --disable-everything --enable-avcodec --enable-avformat --enable-avutil --enable-swscale \
            --enable-swresample --enable-decoder=mp3*,pcm*,aac*,flac,vorbis,opus --enable-parser=mpegaudio,aac \
            --enable-demuxer=mp3,wav,aiff,flac,aac,ogg,pcm* --enable-filter=aresample --enable-protocol=file \
            $ffmpeg_config

make CC="$2 $ffmpeg_cc"
