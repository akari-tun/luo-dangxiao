using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Media;
using System.Windows.Media.Imaging;

namespace luo.dangxiao.common.Utils
{
    public static class ImageUtil
    {
        public static ImageSource ChangePngImageColor(BitmapSource png, System.Windows.Media.Color color)
        {
            var writeableBitmap = new WriteableBitmap(png);

            // 修改所有非透明像素的颜色
            writeableBitmap.Lock();

            try
            {
                IntPtr buffer = writeableBitmap.BackBuffer;
                int stride = writeableBitmap.BackBufferStride;
                int bytesPerPixel = (writeableBitmap.Format.BitsPerPixel + 7) / 8;

                for (int y = 0; y < writeableBitmap.PixelHeight; y++)
                {
                    for (int x = 0; x < writeableBitmap.PixelWidth; x++)
                    {
                        IntPtr pixelPtr = buffer + y * stride + x * bytesPerPixel;

                        // 读取原始像素
                        byte a = Marshal.ReadByte(pixelPtr + 3); // Alpha 通道
                        if (a == 0) continue; // 跳过完全透明像素

                        // 写入新颜色（保留原始透明度）
                        Marshal.WriteByte(pixelPtr, color.B);     // Blue
                        Marshal.WriteByte(pixelPtr + 1, color.G); // Green
                        Marshal.WriteByte(pixelPtr + 2, color.R); // Red
                        Marshal.WriteByte(pixelPtr + 3, (byte)(a * color.A / 255)); // 叠加透明度
                    }
                }

                writeableBitmap.AddDirtyRect(new Int32Rect(0, 0, writeableBitmap.PixelWidth, writeableBitmap.PixelHeight));
            }
            finally
            {
                writeableBitmap.Unlock();
            }

            return writeableBitmap;
        }
    }
}
