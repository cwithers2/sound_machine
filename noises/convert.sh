#!/usr/bin/env bash
#converts target .wav to .mp3
for i in *.wav; do
	ffmpeg -i "$i" "${i%.*}.mp3"
	rm "$i"
done
