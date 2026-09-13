# Validation

## Goal set at project start

Recognition success rate of at least **85 %** on the deployed system.

## Method

The test exercised the whole deployment path, not the Keras model alone:

```
image shown to the board camera -> CH3 224x224 frame -> NPU inference (.nb)
    -> ICPostProcess -> class name on serial monitor + LED state
```

* 20 cabbage images that were **not** used for training, 10 per class.
* Each image was presented to the camera and the printed class name was compared with the true label.
* A run counted as correct when the serial output (and LED) matched the label.

## Result

| Class | Images | Correct |
|---|---:|---:|
| `0_cabbage_good` | 10 | — |
| `1_cabbage_bad` | 10 | — |
| **Total** | **20** | **17 (85 %)** |

The per-class split of the 17 correct results was not recorded separately in the final report, so only the
total is stated here.

The test set was small; a larger set would give a tighter estimate of the true accuracy. The number is
an end-to-end figure and is not the Keras validation accuracy printed during training.

## Observed behavior during the demo

* Healthy cabbage: serial prints `0_cabbage_good`, LED stays off.
* Rotten cabbage: serial prints `class 1, score: ..., name: 1_cabbage_bad` followed by
  `LED ON - ...`, and the blue LED lights up (see `assets/demo_serial_output_disease.jpg`).

## Tomato side experiment

A second model (healthy / mosaic virus / yellow leaf curl virus) was trained with the same script,
converted the same way, and loaded with a 3-entry class list. Two disease images displayed on a monitor
were classified correctly. No quantitative test was run for this model.
