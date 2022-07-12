#!/bin/sh

build() {
    set -o xtrace
	cmake -DBUILD_SHARED_LIBS=False \
          -G Ninja -S . -B ./Build \
    && ninja -C ./Build
}

clean() {
    set -o xtrace
	rm -rf ./Build/*
}

help() {
	echo "Использование:"
	echo "./maker.sh build -> Собрать проект с помощью CMake и ninja -C ./Build/"
	echo "./maker.sh clean -> Удалить содержимое директории ./Build/"
}

case "$1" in
    build) build ;;
    clean) clean ;;
    *) help ;;
esac

