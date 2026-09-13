# Model conversion: `.h5` to `.nb`

The AMB82-mini NPU runs Realtek's `.nb` network binary format. The conversion is done with
the AmebaPro2 AI Convert Model web tool; the result is e-mailed back as a download link.

## Input

Zip `model.h5` (the file name inside the zip must be ASCII).

## Converter settings used

| Field | Value | Reason |
|---|---|---|
| Model type | `CNN-RGB` | The network takes a 224 x 224 x 3 RGB tensor |
| Upload | `model.zip` | Zipped `.h5` |
| `reverse_channel` | `false` | Training used the channel order the camera pipeline delivers; no swap |
| `scale` | `0.00392157` | Equals 1/255, the same normalization applied in `train_model.py` |
| Sample image | one JPG (≤ 1 MB) | Used by the converter to calibrate the quantization range |

## Deploy to the board

1. Download the `.nb` and rename it `imgclassification.nb`.
2. Copy it to an SD card formatted as FAT32 at `NN_MDL/imgclassification.nb`.
3. Insert the SD card and connect the board to the PC.
4. In Arduino IDE: `Tools -> NN Model Load From -> SD Card`.
5. Upload `firmware/plant_disease_classifier`.

The firmware selects the custom model with
`imgclass.modelSelect(IMAGE_CLASSIFICATION, NA_MODEL, NA_MODEL, NA_MODEL, NA_MODEL, CUSTOMIZED_IMGCLASS)`.

## What broke before this pipeline existed

The first plan exported an `.h5` from Teachable Machine. The converter rejected it: Teachable Machine
wraps the network in extra metadata and wrapper layers meant for browser execution, so the converter
cannot locate the plain convolutional graph it expects. Writing the CNN directly in Keras produced a
clean graph the converter accepted.
