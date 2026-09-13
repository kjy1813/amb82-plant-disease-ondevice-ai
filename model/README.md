# Model artifacts

Two artifacts exist per plant, neither is committed:

| File | Size | Produced by | Why not committed |
|---|---|---|---|
| `model.h5` | ~25 MB | `training/train_model.py` | Large binary; regenerable from the script |
| `imgclassification.nb` | ~5.6 MB | AmebaPro2 AI Convert Model (web tool) | Board-specific binary; regenerable from `model.h5` |

Models that were actually built during the project:

| Plant | Classes | Used for |
|---|---|---|
| Cabbage | `0_cabbage_good`, `1_cabbage_bad` | Final demo and the 20-image validation test |
| Tomato | `0_healthy`, `1_Mosaic_virus`, `2_Yellow_Leaf_Curl` | Side experiment; recognized two disease images shown on a monitor |

The firmware in this repository ships with the cabbage class list.

See [`model_conversion.md`](model_conversion.md) for the `.h5` to `.nb` procedure.
