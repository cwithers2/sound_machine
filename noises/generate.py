#!/usr/bin/env python3
#generates white, blue, violet, red, and pink noise mp3s

import numpy as np

SAMPLE_RATE = 44100 #Hz
DURATION    = 600   #seconds

def noise_psd(N, psd = lambda f: 1):
        X_white = np.fft.rfft(np.random.randn(N));
        S = psd(np.fft.rfftfreq(N))
        # Normalize S
        S = S / np.sqrt(np.mean(S**2))
        X_shaped = X_white * S;
        return np.fft.irfft(X_shaped);

def PSDGenerator(f):
    return lambda N: noise_psd(N, f)

@PSDGenerator
def white_noise(f):
    return 1;

@PSDGenerator
def blue_noise(f):
    return np.sqrt(f);

@PSDGenerator
def violet_noise(f):
    return f;

@PSDGenerator
def red_noise(f):
    return 1/np.where(f == 0, float('inf'), f)

@PSDGenerator
def pink_noise(f):
    return 1/np.where(f == 0, float('inf'), np.sqrt(f))


from scipy.io.wavfile import write
import sys

def main():
	funcs = [ white_noise, blue_noise, violet_noise, red_noise, pink_noise]
	files = ["white.wav", "blue.wav", "violet.wav", "red.wav", "pink.wav" ]
	for i, packed in enumerate(zip(funcs, files)):
		func, file = packed
		filename = f"{i+1:>04}{file}"
		sys.stderr.write(f"Writing {filename}\n")
		sys.stderr.flush()
		noise = (func(SAMPLE_RATE * DURATION) * 2**15).astype(np.int16)
		write(filename, SAMPLE_RATE, noise)

if __name__ == "__main__":
	sys.stderr.write("Hello, World!\n")
	main()
	sys.stderr.write("Goodbye, World!\n")
