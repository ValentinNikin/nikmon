#!/bin/bash

fullpath=$(realpath "${BASH_SOURCE}")
workdir=$(dirname -- "${fullpath}")
#mkdir -p "${workdir}/pkgconfig"

#temp_folder=$(mktemp -d -p /tmp)
#mkdir -p ${temp_folder}/pkgconfig

echo ${workdir}

process_libs="${1:-all}"

function hypodermic() {
    if [[ ! -d "${workdir}/hypodermic/" ]]
    then
      cd ${workdir}
      git clone --depth 1 --branch master https://github.com/ybainier/Hypodermic.git
    else
      echo "Skipping hypodermic"
    fi
}

if [[ "${process_libs}" == "hypodermic" ]]; then
    rm -rf "${workdir}/hypodermic"
fi

hypodermic

#rm -rf ${temp_folder}
#
#cd "${workdir}/pkgconfig"
#
#for f in $(ls *.pc);
#do
#    sed -e "s|${temp_folder}|\"${workdir}\"|g" -i ${f}
#done
