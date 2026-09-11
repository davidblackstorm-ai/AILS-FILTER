AILS FILTER HP v1.0
===================

VST3 stereo high-pass filter minimale per Windows 64-bit.
Pensato per Ableton Live 12.

CONTROLLI
- Cutoff: 20 Hz - 20 kHz
- Resonance/Q: 0.50 - 2.00
- Slope: 12 / 24 / 36 / 48 dB per ottava
- Bypass

BUILD SU WINDOWS 11
1. Installa Visual Studio 2022 Community.
2. Durante l'installazione seleziona "Desktop development with C++".
3. Installa CMake (se non viene installato con Visual Studio).
4. Connessione Internet necessaria solo alla prima compilazione: CMake scarica JUCE 8.0.8.
5. Avvia build_windows.bat.
6. Al termine si apre la cartella contenente "AILS Filter HP.vst3".
7. Copia la cartella .vst3 in:
   C:\Program Files\Common Files\VST3\
8. In Ableton Live 12: Preferences > Plug-ins > Rescan.

NOTE TECNICHE
- Solo formato VST3, niente Standalone/AU/AAX.
- DSP stereo/mono.
- Smoothing del cutoff 25 ms per ridurre zipper noise durante le automazioni.
- Nessuna telemetria, rete o dipendenza runtime esterna nel plugin finale.
- Release build con LTO per mantenere il binario compatto.

AILS Audio
