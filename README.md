# MiniEQBoard

## Upload Command

~\AppData\Local\Arduino15\packages\esp32\tools\esptool_py\4.2.1/esptool.exe  --chip esp32c3 --port "COM14" --baud 921600  --before usb_reset --after hard_reset write_flash  -z --flash_mode dio --flash_freq 80m --flash_size 4MB 0x0 ".pio//build//esp32c3//bootloader.bin" 0x8000 ".pio//build//esp32c3//partitions.bin" 0x10000 ".pio//build//esp32c3//firmware.bin" 