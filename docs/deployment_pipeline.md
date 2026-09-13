# Deployment pipeline

## Final pipeline

```
Web-collected leaf images (~300 per class)
        │  resize / normalize, class-per-folder layout
        ▼
training/train_model.py   (Python 3.10, TensorFlow 2.14.1, 30 epochs)
        │  model.h5
        ▼
AmebaPro2 AI Convert Model   (CNN-RGB, scale 1/255, reverse_channel=false, 1 sample image)
        │  imgclassification.nb
        ▼
SD card  NN_MDL/imgclassification.nb   (FAT32)
        │  Arduino IDE: Tools -> NN Model Load From -> SD Card
        ▼
AMB82-mini NPU  <-  firmware/plant_disease_classifier
        │
        ▼
Serial monitor (class, score, name)  +  LED (on = disease)
```

## Initial plan and why it changed

| | Initial plan | Final |
|---|---|---|
| Model source | Teachable Machine (no-code) | Hand-written Keras CNN |
| Export | `.h5` from the web UI | `model.save('model.h5')` |
| Converter result | Rejected: wrapper layers / metadata not understood | Accepted |
| Control over the graph | None | Full (layers, input size, normalization) |

The switch cost time but removed a black box: with the graph written by hand, every converter field
(input size, scale, channel order) could be matched to a known line in the training script.

## Things that had to match exactly

| Training script | Converter field | Firmware |
|---|---|---|
| `target_size=(224, 224)` | 224 x 224 CNN-RGB | `NNWIDTH/NNHEIGHT 224`, `VIDEO_RGB` |
| `rescale=1./255` | `scale = 0.00392157` | (handled inside the `.nb`) |
| RGB channel order | `reverse_channel = false` | `IMAGERGB 1` |
| folder order `0_cabbage_good`, `1_cabbage_bad` | output index 0 / 1 | `imgclassItemList[2]` |

A mismatch in any row produced wrong or constant predictions rather than an error, which is why these
were checked as a set whenever a new model was built.

## Building a new plant model

1. Put images in `dataset/<index>_<name>/`.
2. Set `NUM_CLASSES` in `train_model.py` and run it.
3. Convert `model.h5` with the settings above.
4. Update `ClassificationClassList.h` (array size and names) and re-flash.

This is the procedure that was used for the tomato side experiment.
