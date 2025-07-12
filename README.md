# 🎛️ Multimedia Compression – Image, Audio, and Text Compression Toolkit

This C-based project implements multimodal compression techniques for **lossy compression** of image and audio files and **lossless compression** of text files. It includes custom implementations of:
- 📷 Quad Tree compression for BMP images
- 🔉 Non-linear companding with lookup tables for WAV audio
- 📝 LZW compression for plain text files

---

## 📁 Supported File Types

| Type   | Extension | Compression Type |
|--------|-----------|------------------|
| Image  | `.bmp`    | Lossy            |
| Audio  | `.wav`    | Lossy            |
| Text   | `.txt`    | Lossless         |

---

## 🚀 How to Use

### 🛠️ Compile

```bash
cc *.c
```

▶️ Run the Program

```
./a.out <filename>
```

Examples:

```
./a.out img.bmp     # for image compression
./a.out aud.wav     # for audio compression
./a.out data.txt    # for text compression
```


🧠 Algorithms Used
📷 Image Compression
Quad Tree Decomposition for recursively dividing image regions

Stores average color per quadrant to reduce storage

Decompression via postorder traversal reconstruction

Time Complexity: O(N^2 * log4(N))
Space Complexity: O(N) + O(M^2)


🔉 Audio Compression
Non-linear Companding using lookup tables

Customizable output bits for sample size

Fast encoding with constant time lookup

Time Complexity: O(N)
(N = number of audio samples)


📝 Text Compression
LZW (Lempel-Ziv-Welch) compression for dictionary-based encoding

Dynamically builds a code table while reading text

Efficient for repetitive patterns

Time Complexity: O(k * n)
(n = input length, k = avg. sequence length)


👨‍💻 Contributors

Sandesh Sahane
Priti Patil
Samarth Kanwate
