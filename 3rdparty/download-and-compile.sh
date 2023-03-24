#!/bin/bash

fullpath=$(realpath "${BASH_SOURCE}")
workdir=$(dirname -- "${fullpath}")

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

function nlohmann_json() {
    if [[ ! -d "${workdir}/nlohmann-json/" ]]
    then
        mkdir -p "${workdir}/nlohmann-json"
        cd "${workdir}/nlohmann-json"
        wget https://github.com/nlohmann/json/releases/download/v3.11.2/json.hpp
    else
        echo "Skipping nlohmann-json"
    fi
}

if [[ "${process_libs}" == "hypodermic" ]]; then
    rm -rf "${workdir}/hypodermic"
fi

if [[ "${process_libs}" == "nlohmann-json" ]]; then
    rm -rf "${workdir}/nlohmann-json"
fi

hypodermic
nlohmann_json
