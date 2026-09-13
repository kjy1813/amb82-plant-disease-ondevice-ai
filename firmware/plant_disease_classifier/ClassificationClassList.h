// Class list for the cabbage model (2 classes)
// Course: Embedded Systems Experiment (team project, 2025)
//
// Index must match the output order of imgclassification.nb.
// The tomato experiment used a 3-entry list (0_healthy, 1_Mosaic_virus,
// 2_Yellow_Leaf_Curl); that variant is not part of this submission.

#ifndef __CLASSIFICATIONCLASSLIST_H__
#define __CLASSIFICATIONCLASSLIST_H__

struct ClassificationDetectionItem {
    uint8_t index;
    const char* imgclassName;
    uint8_t filter;
};

ClassificationDetectionItem imgclassItemList[2] = { // edit the class count and names here when the model changes
    {0, "0_cabbage_good", 1},
    {1, "1_cabbage_bad",  1}
};

#endif
