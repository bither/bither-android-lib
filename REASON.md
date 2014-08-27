# Why the image quality of iPhone is much better than Android?

There are so many comparations between Android phone and iPhone. We cannot make the conclusion about which one is better, but we all knows that the image quality of Android phone is much worse than iPhone. No matter you are using Facebook, Twitter or even Instagram, after taking the photo, adding a filter, then sharing to the social network, the images produced by Android phone are always coarse. Why?

Our team had been working on this issue in the last year. After very deep research, we found that this was a "TINY" mistake made by Google. Although tiny, but the influence was very huge (all Android Apps related to image), and lasted till today.

The problem is : [libjpeg].

We all know that [libjpeg] is widely used open source JPEG library. Android also uses libjpeg to compress images. After digging into the source code of Android, we can find that instead of using libjpeg directly, Android is based on an open source image engine called [Skia]. The Skia is a wonderful engine maintained by Google himself, all image functions are implemented in it, and it is widely used by Google and other companies' products (e.g.: Chrome, Firefox, Android......). The Skia has a good encapsulation of libjpeg, you can easily develop image utilites base on this engine.

When using [libjpeg] to compress images, optimize_coding is a very important parameter. In libjpeg.doc, we can find following introductions about this parameter:
```
boolean optimize_coding
	TRUE causes the compressor to compute optimal Huffman coding tables
	for the image.  This requires an extra pass over the data and
	therefore costs a good deal of space and time.  The default is
	FALSE, which tells the compressor to use the supplied or default
	Huffman tables.  In most cases optimal tables save only a few percent
	of file size compared to the default tables.  Note that when this is
	TRUE, you need not supply Huffman tables at all, and any you do
	supply will be overwritten.
```
As the libjpeg.doc, we now know that because setting the optimize_coding to TRUE may cost a good deal of space and time, the default in libjpeg is FALSE.
Everything seems fine about the doc, and libjpeg is very stable. But many people ignored that this document was writen for more than 10 years. At that time, space and computing abilities are very limited. With today's modern computers or even mobile phones, this is not an issue. On the contrary, we should pay more attention to the image quality (retina screens) and image size (cloud services).
Google's engineers of skia project did not set this parameter, so the optimize_coding in Skia was remained to FALSE as the default value, and Skia concealed this setting, you could not change the setting outside of Skia. This became to a big problem, we had to endure worse image and bigger file size.

Our team had tested optimize_coding for many different images. If you want the same quality of image compressing, the file size are 5-10 times bigger when setting the optimze_coding to FALSE than to TRUE. The difference is quite significant.
We also compared the jpeg compressing between iOS and Android (they both concealed the optimize_coding parameter). With the same original images, if you want same quality level, you need 5-10 times file size on Android.
The result is clear, Apple does know the importance of optimize_coding and Huffman tables and Google does not. (Apple uses their own Huffman table algorithm, not like libjpeg or libjpeg-turbo. It seems that Apple has done more tuning works on image compressing.)

Finally, we decided not to use JPEG compress functions provided by Android, and we compiled our own native library based on libjpeg-turbo (libjpeg-turbo also has performance improvements). Now we can save 5-10 times of image space and enjoy the same or even better image quality. This work is totally worth to do.

Thanks for reading, :)

Our project on github:
https://github.com/bither

[libjpeg]:(http://en.wikipedia.org/wiki/Libjpeg)
[Skia]:(http://en.wikipedia.org/wiki/Skia_Graphics_Engine)
