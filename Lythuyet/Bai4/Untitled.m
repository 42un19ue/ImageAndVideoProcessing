clc
clear

quantizationTable = [
    16  11  10  16  24  40  51  61
    12  12  14  19  26  58  60  55
    14  13  16  24  40  57  69  56
    14  17  22  29  51  87  80  62
    18  22  37  56  68 109 103  77
    24  35  55  64  81 104 113  92
    49  64  78  87 103 121 120 101
    72  92  95  98 112 100 103  99
];

image = imread('8x8.jpg');
%image = imread('256x256.jpg');
%image = imread('1280x960.jpg');
source = uint8(image(:,:,1));

[rows, cols] = size(source);
larger = [rows, cols];
largeTable = repmat(quantizationTable, larger ./ size(quantizationTable));

dct = dct2(source);
normalizedquantizedDCT = round(dct ./ largeTable);
denormalizedquantizedDCT = normalizedquantizedDCT .* largeTable;
reconstructed = round(idct2(denormalizedquantizedDCT));

figure;

subplot(3, 2, 1);
imshow(image);
title('Orginal Image');

subplot(3, 2, 2);
imshow(source);
title('Grayscale Image');

subplot(3, 2, 3);
imshow(log(abs(dct) + 1), []);
title('Forward DCT Coefficients (log scale)');

subplot(3, 2, 4);
imshow(uint8(normalizedquantizedDCT));
title('Normalized Quantized Image');

subplot(3, 2, 5);
imshow(denormalizedquantizedDCT);
title('Denormalized Quantized Image');

subplot(3, 2, 6);
imshow(uint8(reconstructed));
title('Reconstructed Image');
