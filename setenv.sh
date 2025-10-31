ROOT_PATH=~/pi_pico

export PICO_SDK_PATH=$ROOT_PATH/pico-sdk
export PICO_EXAMPLES_PATH=$ROOT_PATH/pico-examples
export PICO_EXTRAS_PATH=$ROOT_PATH/pico-extras
export PICO_PLAYGROUND_PATH=$ROOT_PATH/pico-playground

export PICO_PLATFORM=rp2040

export PICO_TOOLCHAIN_PATH=$ROOT_PATH/gcc-arm-none-eabi-10.3-2021.10/bin

if [ ! -d $ROOT_PATH/FreeRTOS-Kernel ]; then
    echo "FreeRTOS-Kernel is not present"
else
    export FREERTOS_KERNEL_PATH=$ROOT_PATH/FreeRTOS-Kernel
fi

if [ ! -f $ROOT_PATH/picotool/build/picotool ]; then
        echo picotool is not present
elif [[ $PATH != *"$ROOT_PATH/picotool/build"* ]]; then
        export PATH=$PATH:$ROOT_PATH/picotool/build
fi

