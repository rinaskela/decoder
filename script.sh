#! bin/bash
cd ~/mpdroot/build
make -j12
. config.sh
cd ~/mpdroot/decoder
root -l .x MpdDataToRoot.C