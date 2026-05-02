# DICOM Conformance Statement — AlizaMS

## 1. Introduction

### 1.1 Product Description

| Field | Value |
|-------|-------|
| Product Name | AlizaMS |
| Version | 1.9.12 |
| Classification | DICOM Medical Image Viewer |
| Manufacturer | Open Source (MDCM library) |
| Document Date | 2026-05-02 |

AlizaMS is a desktop DICOM medical image viewer application. It reads and
displays DICOM files from local storage (file-based media). AlizaMS acts as a
**File Set Reader (FSR)** — it consumes DICOM objects but does not create,
modify, or transmit them over a network.

This document describes the DICOM conformance of AlizaMS in accordance with
DICOM PS3.2 (Conformance).

### 1.2 Applicable Standards

- DICOM PS3.2 — Conformance
- DICOM PS3.3 — Information Object Definitions
- DICOM PS3.4 — Service Class Specifications
- DICOM PS3.5 — Data Structures and Encoding
- DICOM PS3.6 — Data Dictionary
- DICOM PS3.10 — Media Storage and File Format
- DICOM PS3.12 — Media Formats and Physical Media

### 1.3 Networking Support

AlizaMS does **not** implement DICOM network communication (DIMSE services).
It does not function as an SCU or SCP over a network. All DICOM object
ingestion is performed via local file system access (DICOM Part 10 files and
DICOMDIR).

### 1.4 Supported IOD Categories

AlizaMS supports reading the following IOD categories:

| IOD Category | Support Level |
|---|---|
| Image IODs (CT, MR, US, XA, NM, PET, CR, DX, MG, RT Image, etc.) | Full display |
| Structured Report (SR) — Basic Text, Enhanced, Comprehensive | Read / display |
| Presentation State (Grayscale Softcopy — GSPS) | Read / apply |
| Key Object Selection Document | Read / display |
| RT Structure Set | Read / display |
| RT Dose | Read / display |
| RT Plan, RT Ion Plan | Read (metadata) |
| Segmentation | Read / display |
| Encapsulated PDF | Read / display |
| Parametric Map | Read / display |
| DICOMDIR (Media Storage Directory) | Read / navigate |

## 2. Supported SOP Classes

AlizaMS reads (accepts) the following SOP Classes. The role is **User Agent
(reader)** — equivalent to an SCP role for file-based media exchange.

| # | SOP Class Name | SOP Class UID |
|---|---|---|
| 1 | Media Storage Directory Storage | 1.2.840.10008.1.3.10 |
| 2 | Computed Radiography Image Storage | 1.2.840.10008.5.1.4.1.1.1 |
| 3 | Digital X-Ray Image Storage — For Presentation | 1.2.840.10008.5.1.4.1.1.1.1 |
| 4 | Digital X-Ray Image Storage — For Processing | 1.2.840.10008.5.1.4.1.1.1.1.1 |
| 5 | Digital Mammography Image Storage — For Presentation | 1.2.840.10008.5.1.4.1.1.1.2 |
| 6 | Digital Mammography Image Storage — For Processing | 1.2.840.10008.5.1.4.1.1.1.2.1 |
| 7 | Digital Intra-Oral X-Ray Image Storage — For Presentation | 1.2.840.10008.5.1.4.1.1.1.3 |
| 8 | Digital Intra-Oral X-Ray Image Storage — For Processing | 1.2.840.10008.5.1.4.1.1.1.3.1 |
| 9 | CT Image Storage | 1.2.840.10008.5.1.4.1.1.2 |
| 10 | Enhanced CT Image Storage | 1.2.840.10008.5.1.4.1.1.2.1 |
| 11 | Ultrasound Image Storage (Retired) | 1.2.840.10008.5.1.4.1.1.6 |
| 12 | Ultrasound Image Storage | 1.2.840.10008.5.1.4.1.1.6.1 |
| 13 | Ultrasound Multi-frame Image Storage (Retired) | 1.2.840.10008.5.1.4.1.1.3 |
| 14 | Ultrasound Multi-frame Image Storage | 1.2.840.10008.5.1.4.1.1.3.1 |
| 15 | MR Image Storage | 1.2.840.10008.5.1.4.1.1.4 |
| 16 | Enhanced MR Image Storage | 1.2.840.10008.5.1.4.1.1.4.1 |
| 17 | MR Spectroscopy Storage | 1.2.840.10008.5.1.4.1.1.4.2 |
| 18 | Nuclear Medicine Image Storage (Retired) | 1.2.840.10008.5.1.4.1.1.5 |
| 19 | Secondary Capture Image Storage | 1.2.840.10008.5.1.4.1.1.7 |
| 20 | Multi-frame Single Bit Secondary Capture Image Storage | 1.2.840.10008.5.1.4.1.1.7.1 |
| 21 | Multi-frame Grayscale Byte Secondary Capture Image Storage | 1.2.840.10008.5.1.4.1.1.7.2 |
| 22 | Multi-frame Grayscale Word Secondary Capture Image Storage | 1.2.840.10008.5.1.4.1.1.7.3 |
| 23 | Multi-frame True Color Secondary Capture Image Storage | 1.2.840.10008.5.1.4.1.1.7.4 |
| 24 | Standalone Overlay Storage (Retired) | 1.2.840.10008.5.1.4.1.1.8 |
| 25 | Standalone Curve Storage (Retired) | 1.2.840.10008.5.1.4.1.1.9 |
| 26 | 12-Lead ECG Waveform Storage | 1.2.840.10008.5.1.4.1.1.9.1.1 |
| 27 | General ECG Waveform Storage | 1.2.840.10008.5.1.4.1.1.9.1.2 |
| 28 | Ambulatory ECG Waveform Storage | 1.2.840.10008.5.1.4.1.1.9.1.3 |
| 29 | Hemodynamic Waveform Storage | 1.2.840.10008.5.1.4.1.1.9.2.1 |
| 30 | Cardiac Electrophysiology Waveform Storage | 1.2.840.10008.5.1.4.1.1.9.3.1 |
| 31 | Basic Voice Audio Waveform Storage | 1.2.840.10008.5.1.4.1.1.9.4.1 |
| 32 | Standalone Modality LUT Storage (Retired) | 1.2.840.10008.5.1.4.1.1.10 |
| 33 | Standalone VOI LUT Storage (Retired) | 1.2.840.10008.5.1.4.1.1.11 |
| 34 | Grayscale Softcopy Presentation State Storage | 1.2.840.10008.5.1.4.1.1.11.1 |
| 35 | X-Ray Angiographic Image Storage | 1.2.840.10008.5.1.4.1.1.12.1 |
| 36 | X-Ray Radiofluoroscopic Image Storage | 1.2.840.10008.5.1.4.1.1.12.2 |
| 37 | X-Ray Angiographic Bi-Plane Image Storage (Retired) | 1.2.840.10008.5.1.4.1.1.12.3 |
| 38 | Nuclear Medicine Image Storage | 1.2.840.10008.5.1.4.1.1.20 |
| 39 | Raw Data Storage | 1.2.840.10008.5.1.4.1.1.66 |
| 40 | Spatial Registration Storage | 1.2.840.10008.5.1.4.1.1.66.1 |
| 41 | Spatial Fiducials Storage | 1.2.840.10008.5.1.4.1.1.66.2 |
| 42 | PET Image Storage | 1.2.840.10008.5.1.4.1.1.128 |
| 43 | RT Image Storage | 1.2.840.10008.5.1.4.1.1.481.1 |
| 44 | RT Dose Storage | 1.2.840.10008.5.1.4.1.1.481.2 |
| 45 | RT Structure Set Storage | 1.2.840.10008.5.1.4.1.1.481.3 |
| 46 | RT Plan Storage | 1.2.840.10008.5.1.4.1.1.481.5 |
| 47 | CSA Non-Image Storage (Siemens Private) | 1.3.12.2.1107.5.9.1 |
| 48 | Philips 3D (Private) | 1.2.840.113543.6.6.1.3.10002 |
| 49 | Enhanced SR Storage | 1.2.840.10008.5.1.4.1.1.88.22 |
| 50 | Basic Text SR Storage | 1.2.840.10008.5.1.4.1.1.88.11 |
| 51 | Hardcopy Grayscale Image Storage (Retired) | 1.2.840.10008.5.1.1.29 |
| 52 | Comprehensive SR Storage | 1.2.840.10008.5.1.4.1.1.88.33 |
| 53 | Detached Study Management SOP Class (Retired) | 1.2.840.10008.3.1.2.3.1 |
| 54 | Encapsulated PDF Storage | 1.2.840.10008.5.1.4.1.1.104.1 |
| 55 | Encapsulated CDA Storage | 1.2.840.10008.5.1.4.1.1.104.2 |
| 56 | Study Component Management SOP Class (Retired) | 1.2.840.10008.3.1.2.3.2 |
| 57 | Detached Visit Management SOP Class (Retired) | 1.2.840.10008.3.1.2.2.1 |
| 58 | Detached Patient Management SOP Class (Retired) | 1.2.840.10008.3.1.2.1.1 |
| 59 | Video Endoscopic Image Storage | 1.2.840.10008.5.1.4.1.1.77.1.1.1 |
| 60 | GE Magnetic Resonance Image Storage (Private) | 1.2.840.113619.4.2 |
| 61 | GE Private 3D Model Storage (Private) | 1.2.840.113619.4.26 |
| 62 | Toshiba Private Data Storage (Private) | 1.2.392.200036.9116.7.8.1.1.1 |
| 63 | Mammography CAD SR Storage | 1.2.840.10008.5.1.4.1.1.88.50 |
| 64 | Key Object Selection Document | 1.2.840.10008.5.1.4.1.1.88.59 |
| 65 | Hanging Protocol Storage | 1.2.840.10008.5.1.4.38.1 |
| 66 | Modality Performed Procedure Step SOP Class | 1.2.840.10008.3.1.2.3.3 |
| 67 | Philips Private MR Synthetic Image Storage (Private) | 1.3.46.670589.5.0.10 |
| 68 | VL Photographic Image Storage | 1.2.840.10008.5.1.4.1.1.77.1.4 |
| 69 | Segmentation Storage | 1.2.840.10008.5.1.4.1.1.66.4 |
| 70 | RT Ion Plan Storage | 1.2.840.10008.5.1.4.1.1.481.8 |
| 71 | X-Ray 3D Angiographic Image Storage | 1.2.840.10008.5.1.4.1.1.13.1.1 |
| 72 | Enhanced XA Image Storage | 1.2.840.10008.5.1.4.1.1.12.1.1 |
| 73 | RT Ion Beams Treatment Record Storage | 1.2.840.10008.5.1.4.1.1.481.9 |
| 74 | Surface Segmentation Storage | 1.2.840.10008.5.1.4.1.1.66.5 |
| 75 | VL Whole Slide Microscopy Image Storage | 1.2.840.10008.5.1.4.1.1.77.1.6 |
| 76 | RT Treatment Summary Record Storage | 1.2.840.10008.5.1.4.1.1.481.7 |
| 77 | Enhanced US Volume Storage | 1.2.840.10008.5.1.4.1.1.6.2 |
| 78 | X-Ray Radiation Dose SR Storage | 1.2.840.10008.5.1.4.1.1.88.67 |
| 79 | VL Endoscopic Image Storage | 1.2.840.10008.5.1.4.1.1.77.1.1 |
| 80 | Breast Tomosynthesis Image Storage | 1.2.840.10008.5.1.4.1.1.13.1.3 |
| 81 | Fuji Private CR Image Storage (Private) | 1.2.392.200036.9125.1.1.2 |
| 82 | Ophthalmic Photography 8-Bit Image Storage | 1.2.840.10008.5.1.4.1.1.77.1.5.1 |
| 83 | Ophthalmic Photography 16-Bit Image Storage | 1.2.840.10008.5.1.4.1.1.77.1.5.2 |
| 84 | Ophthalmic Tomography Image Storage | 1.2.840.10008.5.1.4.1.1.77.1.5.4 |
| 85 | VL Microscopic Image Storage | 1.2.840.10008.5.1.4.1.1.77.1.2 |
| 86 | Enhanced PET Image Storage | 1.2.840.10008.5.1.4.1.1.130 |
| 87 | Video Photographic Image Storage | 1.2.840.10008.5.1.4.1.1.77.1.4.1 |
| 88 | X-Ray 3D Craniofacial Image Storage | 1.2.840.10008.5.1.4.1.1.13.1.2 |
| 89 | Intravascular OCT Image Storage — For Presentation | 1.2.840.10008.5.1.4.1.1.14.1 |
| 90 | Intravascular OCT Image Storage — For Processing | 1.2.840.10008.5.1.4.1.1.14.2 |
| 91 | Legacy Converted Enhanced CT Image Storage | 1.2.840.10008.5.1.4.1.1.2.2 |
| 92 | Legacy Converted Enhanced MR Image Storage | 1.2.840.10008.5.1.4.1.1.4.4 |
| 93 | Legacy Converted Enhanced PET Image Storage | 1.2.840.10008.5.1.4.1.1.128.1 |
| 94 | Surface Scan Mesh Storage | 1.2.840.10008.5.1.4.1.1.68.1 |
| 95 | Enhanced MR Color Image Storage | 1.2.840.10008.5.1.4.1.1.4.3 |
| 96 | Parametric Map Storage | 1.2.840.10008.5.1.4.1.1.30 |
| 97 | Breast Projection X-Ray Image Storage — For Presentation | 1.2.840.10008.5.1.4.1.1.13.1.4 |
| 98 | Breast Projection X-Ray Image Storage — For Processing | 1.2.840.10008.5.1.4.1.1.13.1.5 |
| 99 | Hardcopy Color Image Storage (Retired) | 1.2.840.10008.5.1.1.30 |
| 100 | Photoacoustic Image Storage | 1.2.840.10008.5.1.4.1.1.6.3 |

## 3. Supported Transfer Syntaxes

| # | Transfer Syntax Name | Transfer Syntax UID |
|---|---|---|
| 1 | Implicit VR Little Endian | 1.2.840.10008.1.2 |
| 2 | Implicit VR Big Endian (GE Private) | 1.2.840.113619.5.2 |
| 3 | Explicit VR Little Endian | 1.2.840.10008.1.2.1 |
| 4 | Deflated Explicit VR Little Endian | 1.2.840.10008.1.2.1.99 |
| 5 | Explicit VR Big Endian | 1.2.840.10008.1.2.2 |
| 6 | JPEG Baseline (Process 1) | 1.2.840.10008.1.2.4.50 |
| 7 | JPEG Extended (Process 2 & 4) | 1.2.840.10008.1.2.4.51 |
| 8 | JPEG Extended (Process 3 & 5) | 1.2.840.10008.1.2.4.52 |
| 9 | JPEG Spectral Selection, Non-Hierarchical (Process 6 & 8) | 1.2.840.10008.1.2.4.53 |
| 10 | JPEG Full Progression, Non-Hierarchical (Process 10 & 12) | 1.2.840.10008.1.2.4.55 |
| 11 | JPEG Full Progression, Non-Hierarchical (Process 11 & 13) | 1.2.840.10008.1.2.4.56 |
| 12 | JPEG Lossless, Non-Hierarchical (Process 14) | 1.2.840.10008.1.2.4.57 |
| 13 | JPEG Lossless, Non-Hierarchical (Process 15) | 1.2.840.10008.1.2.4.58 |
| 14 | JPEG Extended, Hierarchical (Process 16 & 18) | 1.2.840.10008.1.2.4.59 |
| 15 | JPEG Extended, Hierarchical (Process 17 & 19) | 1.2.840.10008.1.2.4.60 |
| 16 | JPEG Spectral Selection, Hierarchical (Process 20 & 22) | 1.2.840.10008.1.2.4.61 |
| 17 | JPEG Spectral Selection, Hierarchical (Process 21 & 23) | 1.2.840.10008.1.2.4.62 |
| 18 | JPEG Full Progression, Hierarchical (Process 24 & 26) | 1.2.840.10008.1.2.4.63 |
| 19 | JPEG Full Progression, Hierarchical (Process 25 & 27) | 1.2.840.10008.1.2.4.64 |
| 20 | JPEG Lossless, Hierarchical (Process 28) | 1.2.840.10008.1.2.4.65 |
| 21 | JPEG Lossless, Hierarchical (Process 29) | 1.2.840.10008.1.2.4.66 |
| 22 | JPEG Lossless, First-Order Prediction (Process 14, Selection Value 1) | 1.2.840.10008.1.2.4.70 |
| 23 | JPEG-LS Lossless Image Compression | 1.2.840.10008.1.2.4.80 |
| 24 | JPEG-LS Lossy (Near-Lossless) Image Compression | 1.2.840.10008.1.2.4.81 |
| 25 | JPEG 2000 Image Compression (Lossless Only) | 1.2.840.10008.1.2.4.90 |
| 26 | JPEG 2000 Image Compression | 1.2.840.10008.1.2.4.91 |
| 27 | JPEG 2000 Part 2 Multi-component Image Compression (Lossless Only) | 1.2.840.10008.1.2.4.92 |
| 28 | JPEG 2000 Part 2 Multi-component Image Compression | 1.2.840.10008.1.2.4.93 |
| 29 | RLE Lossless | 1.2.840.10008.1.2.5 |
| 30 | MPEG2 Main Profile / Main Level | 1.2.840.10008.1.2.4.100 |
| 31 | Implicit VR Big Endian (ACR-NEMA) | (internal) |
| 32 | Papyrus (internal) | (internal) |
| 33 | CT Private ELE (Philips Private) | 1.3.46.670589.33.1.4.1 |
| 34 | JPIP Referenced | 1.2.840.10008.1.2.4.94 |
| 35 | JPIP Referenced Deflate | 1.2.840.10008.1.2.4.95 |
| 36 | MPEG2 Main Profile / High Level | 1.2.840.10008.1.2.4.101 |
| 37 | MPEG-4 AVC/H.264 High Profile / Level 4.1 | 1.2.840.10008.1.2.4.102 |
| 38 | MPEG-4 AVC/H.264 BD-compatible High Profile / Level 4.1 | 1.2.840.10008.1.2.4.103 |
| 39 | Encapsulated Uncompressed Explicit VR Little Endian | 1.2.840.10008.1.2.1.98 |
| 40 | SMPTE ST 2110-20 Uncompressed Progressive Active Video | 1.2.840.10008.1.2.7.1 |
| 41 | SMPTE ST 2110-20 Uncompressed Interlaced Active Video | 1.2.840.10008.1.2.7.2 |
| 42 | SMPTE ST 2110-30 PCM Digital Audio | 1.2.840.10008.1.2.7.3 |
| 43 | MPEG-4 AVC/H.264 High Profile / Level 4.2 For 2D Video | 1.2.840.10008.1.2.4.104 |
| 44 | MPEG-4 AVC/H.264 High Profile / Level 4.2 For 3D Video | 1.2.840.10008.1.2.4.105 |
| 45 | MPEG-4 AVC/H.264 Stereo High Profile / Level 4.2 | 1.2.840.10008.1.2.4.106 |
| 46 | HEVC/H.265 Main Profile / Level 5.1 | 1.2.840.10008.1.2.4.107 |
| 47 | HEVC/H.265 Main 10 Profile / Level 5.1 | 1.2.840.10008.1.2.4.108 |
| 48 | Fragmentable MPEG2 Main Profile / Main Level | 1.2.840.10008.1.2.4.100.1 |
| 49 | Fragmentable MPEG2 Main Profile / High Level | 1.2.840.10008.1.2.4.101.1 |
| 50 | Fragmentable MPEG-4 AVC/H.264 High Profile / Level 4.1 | 1.2.840.10008.1.2.4.102.1 |
| 51 | Fragmentable MPEG-4 AVC/H.264 BD-compatible High Profile / Level 4.1 | 1.2.840.10008.1.2.4.103.1 |
| 52 | Fragmentable MPEG-4 AVC/H.264 High Profile / Level 4.2 For 2D Video | 1.2.840.10008.1.2.4.104.1 |
| 53 | Fragmentable MPEG-4 AVC/H.264 High Profile / Level 4.2 For 3D Video | 1.2.840.10008.1.2.4.105.1 |
| 54 | Fragmentable MPEG-4 AVC/H.264 Stereo High Profile / Level 4.2 | 1.2.840.10008.1.2.4.106.1 |
| 55 | High-Throughput JPEG 2000 (Lossless Only) | 1.2.840.10008.1.2.4.201 |
| 56 | High-Throughput JPEG 2000 with RPCL Options (Lossless Only) | 1.2.840.10008.1.2.4.202 |
| 57 | High-Throughput JPEG 2000 | 1.2.840.10008.1.2.4.203 |
| 58 | JPIP HTJ2K Referenced | 1.2.840.10008.1.2.4.204 |
| 59 | JPIP HTJ2K Referenced Deflate | 1.2.840.10008.1.2.4.205 |
| 60 | JPEG XL Lossless | 1.2.840.10008.1.2.4.110 |
| 61 | JPEG XL JPEG Recompression | 1.2.840.10008.1.2.4.111 |
| 62 | JPEG XL | 1.2.840.10008.1.2.4.112 |

## 4. Supported Character Sets

AlizaMS supports the following DICOM Specific Character Sets via Qt text
codec conversion. ISO 2022 code extension techniques (escape sequences for
multi-valued character sets) are supported.

| DICOM Defined Term | Description | Encoding |
|---|---|---|
| (empty / default) | Default repertoire (ASCII) | ISO-IR 6 |
| ISO_IR 6 | ASCII | US-ASCII |
| ISO_IR 100 / ISO 2022 IR 100 | Latin Alphabet No. 1 | ISO-8859-1 |
| ISO_IR 101 / ISO 2022 IR 101 | Latin Alphabet No. 2 | ISO-8859-2 |
| ISO_IR 109 / ISO 2022 IR 109 | Latin Alphabet No. 3 | ISO-8859-3 |
| ISO_IR 110 / ISO 2022 IR 110 | Latin Alphabet No. 4 | ISO-8859-4 |
| ISO_IR 144 / ISO 2022 IR 144 | Cyrillic | ISO-8859-5 |
| ISO_IR 127 / ISO 2022 IR 127 | Arabic | ISO-8859-6 |
| ISO_IR 126 / ISO 2022 IR 126 | Greek | ISO-8859-7 |
| ISO_IR 138 / ISO 2022 IR 138 | Hebrew | ISO-8859-8 |
| ISO_IR 148 / ISO 2022 IR 148 | Latin Alphabet No. 5 (Turkish) | ISO-8859-9 |
| ISO_IR 166 / ISO 2022 IR 166 | Thai | TIS-620 |
| ISO_IR 13 / ISO 2022 IR 13 | Japanese (Katakana) | Shift_JIS |
| ISO 2022 IR 87 | Japanese (Kanji) | ISO-2022-JP |
| ISO 2022 IR 159 | Japanese (Supplementary Kanji) | ISO-2022-JP |
| ISO 2022 IR 149 | Korean | EUC-KR / iso-ir-149 |
| ISO 2022 IR 58 | Simplified Chinese | GB2312 |
| ISO_IR 192 | Unicode UTF-8 | UTF-8 |
| GB18030 | Chinese (GB18030) | GB18030 |
| GBK | Chinese (GBK) | GBK |

AlizaMS also provides a user-configurable option to force Windows-1251
(Cyrillic) decoding for files with an empty Specific Character Set value.

## 5. Media Storage Application Profile

### 5.1 Implementation Details

| Field | Value |
|---|---|
| Media Storage Application Profile | General Purpose Media Storage (PS3.12) |
| File Format | DICOM Part 10 File Format |
| DICOMDIR Support | Yes (read) |
| File Meta Information | Read and parsed |

### 5.2 Physical Media

AlizaMS reads DICOM files from any locally accessible file system, including
local disks, network-mounted volumes, and removable media (USB, CD/DVD).

## 6. Security

AlizaMS does not implement DICOM security profiles. It does not perform
encryption, digital signatures, or access control beyond the operating
system's file permissions.

## 7. Annexes

### 7.1 Private SOP Classes

AlizaMS provides best-effort support for the following private (non-standard)
SOP Classes. These are parsed and displayed when possible but are not
guaranteed to render correctly.

| Vendor | SOP Class Name | UID |
|---|---|---|
| Siemens | CSA Non-Image Storage | 1.3.12.2.1107.5.9.1 |
| Philips | Philips 3D | 1.2.840.113543.6.6.1.3.10002 |
| GE | GE Magnetic Resonance Image Storage | 1.2.840.113619.4.2 |
| GE | GE Private 3D Model Storage | 1.2.840.113619.4.26 |
| Toshiba | Toshiba Private Data Storage | 1.2.392.200036.9116.7.8.1.1.1 |
| Fuji | Fuji Private CR Image Storage | 1.2.392.200036.9125.1.1.2 |
| Philips | Philips Private MR Synthetic Image Storage | 1.3.46.670589.5.0.10 |
| Philips | CT Private ELE (Transfer Syntax) | 1.3.46.670589.33.1.4.1 |

### 7.2 Retired SOP Classes

The following retired SOP Classes are accepted for backward compatibility:

- Ultrasound Image Storage (Retired) — 1.2.840.10008.5.1.4.1.1.6
- Ultrasound Multi-frame Image Storage (Retired) — 1.2.840.10008.5.1.4.1.1.3
- Nuclear Medicine Image Storage (Retired) — 1.2.840.10008.5.1.4.1.1.5
- Standalone Overlay Storage (Retired) — 1.2.840.10008.5.1.4.1.1.8
- Standalone Curve Storage (Retired) — 1.2.840.10008.5.1.4.1.1.9
- Standalone Modality LUT Storage (Retired) — 1.2.840.10008.5.1.4.1.1.10
- Standalone VOI LUT Storage (Retired) — 1.2.840.10008.5.1.4.1.1.11
- X-Ray Angiographic Bi-Plane Image Storage (Retired) — 1.2.840.10008.5.1.4.1.1.12.3
- Hardcopy Grayscale Image Storage (Retired) — 1.2.840.10008.5.1.1.29
- Hardcopy Color Image Storage (Retired) — 1.2.840.10008.5.1.1.30
- Detached Study Management SOP Class (Retired) — 1.2.840.10008.3.1.2.3.1
- Study Component Management SOP Class (Retired) — 1.2.840.10008.3.1.2.3.2
- Detached Visit Management SOP Class (Retired) — 1.2.840.10008.3.1.2.2.1
- Detached Patient Management SOP Class (Retired) — 1.2.840.10008.3.1.2.1.1

### 7.3 Limitations

- No DICOM network communication (C-STORE, C-FIND, C-MOVE, C-GET, etc.)
- No DICOM Query/Retrieve services
- No DICOM Worklist Management
- No DICOM Storage Commitment
- No DICOM Print Management
- Read-only — AlizaMS does not create or modify DICOM objects
- Presentation State support is limited to Grayscale Softcopy (GSPS)
- Waveform objects are parsed but display may be limited