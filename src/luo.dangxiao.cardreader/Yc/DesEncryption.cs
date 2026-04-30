using System;

namespace luo.dangxiao.cardreader.Yc;

/// <summary>
/// Custom DES implementation matching the YCCARD C++ library.
/// This is a bit-level DES implementation with specific tables.
/// </summary>
public static class DesEncryption
{
    // Initial Permutation Table (IP)
    private static readonly byte[] InitialTr = new byte[]
    {
        58,50,42,34,26,18,10,02,
        60,52,44,36,28,20,12,04,
        62,54,46,38,30,22,14,06,
        64,56,48,40,32,24,16,0x08,
        57,49,41,33,25,17,0x09,01,
        59,51,43,35,27,19,11,03,
        61,53,45,37,29,21,13,05,
        63,55,47,39,31,23,15,07
    };

    // Final Permutation Table (IP^-1)
    private static readonly byte[] FinalTr = new byte[]
    {
        40,0x08,48,16,56,24,64,32,
        39,07,47,15,55,23,63,31,
        38,06,46,14,54,22,62,30,
        37,05,45,13,53,21,61,29,
        36,04,44,12,52,20,60,28,
        35,03,43,11,51,19,59,27,
        34,02,42,10,50,18,58,26,
        33,01,41,0x09,49,17,57,25
    };

    // Swap table for Feistel network
    private static readonly byte[] Swap = new byte[]
    {
        33,34,35,36,37,38,39,40,
        41,42,43,44,45,46,47,48,
        49,50,51,52,53,54,55,56,
        57,58,59,60,61,62,63,64,
        01,02,03,04,05,06,07,0x08,
        0x09,10,11,12,13,14,15,16,
        17,18,19,20,21,22,23,24,
        25,26,27,28,29,30,31,32
    };

    // Key Permutation Table 1 (PC-1)
    private static readonly byte[] KeyTr1 = new byte[]
    {
        57,49,41,33,25,17,0x09,
        01,58,50,42,34,26,18,
        10,02,59,51,43,35,27,
        19,11,03,60,52,44,36,
        63,55,47,39,31,23,15,
        07,62,54,46,38,30,22,
        14,06,61,53,45,37,29,
        21,13,05,28,20,12,04,
        00,00,00,00,00,00,00,00
    };

    // Key Permutation Table 2 (PC-2)
    private static readonly byte[] KeyTr2 = new byte[]
    {
        14,17,11,24,01,05,
        03,28,15,06,21,10,
        23,19,12,04,26,0x08,
        16,07,27,20,13,02,
        41,52,31,37,47,55,
        30,40,51,45,33,48,
        44,49,39,56,34,53,
        46,42,50,36,29,32,
        00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00
    };

    // Expansion Table (E)
    private static readonly byte[] Etr = new byte[]
    {
        32,01,02,03,04,05,
        04,05,06,07,0x08,0x09,
        0x08,0x09,10,11,12,13,
        12,13,14,15,16,17,
        16,17,18,19,20,21,
        20,21,22,23,24,25,
        24,25,26,27,28,29,
        28,29,30,31,32,01,
        00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00
    };

    // Permutation Table (P)
    private static readonly byte[] Ptr = new byte[]
    {
        16,07,20,21,
        29,12,28,17,
        01,15,23,26,
        05,18,31,10,
        02,0x08,24,14,
        32,27,03,0x09,
        19,13,30,06,
        22,11,04,25,
        00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,
        00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00
    };

    // S-Boxes (8 boxes, each 64 entries)
    private static readonly byte[][] S = new byte[][]
    {
        new byte[] {14,04,13,01,02,15,11,0x08,03,10,06,12,05,0x09,00,07,
                    00,15,07,04,14,02,13,01,10,06,12,11,0x09,05,03,0x08,
                    01,01,14,0x08,13,06,02,11,15,12,0x09,07,03,10,05,00,
                    15,12,0x08,02,04,0x09,01,07,05,11,03,14,10,00,06,13},
        new byte[] {15,01,0x08,14,06,11,03,04,0x09,07,02,13,12,00,05,10,
                    03,13,04,07,15,02,0x08,15,12,00,01,10,06,0x09,11,05,
                    00,14,17,11,10,04,13,01,05,0x08,12,06,0x09,03,02,15,
                    13,0x08,10,01,03,15,04,02,11,06,07,12,00,05,14,0x09},
        new byte[] {10,00,0x09,14,06,03,15,05,01,13,12,07,11,04,02,0x08,
                    13,07,00,0x09,03,04,06,10,02,0x08,05,14,12,11,15,01,
                    13,06,04,0x09,0x08,15,03,00,11,01,02,12,05,10,14,07,
                    01,10,13,00,06,0x09,0x08,07,04,15,14,03,11,05,02,12},
        new byte[] {07,13,14,03,00,06,0x09,10,01,02,0x08,05,11,12,04,15,
                    13,0x08,11,05,06,15,00,03,04,07,02,12,01,10,14,0x09,
                    10,06,0x09,00,12,11,07,13,15,01,03,14,05,02,0x08,04,
                    03,15,00,06,10,10,13,0x08,0x09,04,05,11,12,07,02,14},
        new byte[] {02,12,04,01,07,10,11,06,0x08,05,03,15,13,00,14,0x09,
                    14,11,02,12,04,07,13,01,05,00,15,10,03,0x09,0x08,06,
                    04,02,01,11,10,13,07,0x08,15,0x09,12,05,06,03,00,14,
                    11,0x08,12,07,01,14,02,13,06,15,00,0x09,10,04,05,03},
        new byte[] {12,01,10,15,0x09,02,06,0x08,00,13,03,04,14,07,05,11,
                    10,15,04,02,07,12,0x09,05,06,01,13,14,00,11,03,0x08,
                    0x09,14,15,05,02,0x08,12,03,07,00,04,10,01,13,11,06,
                    04,03,02,12,0x09,05,15,10,11,14,01,07,06,00,0x08,13},
        new byte[] {04,11,02,14,15,00,0x08,13,03,12,0x09,07,05,10,06,01,
                    13,00,11,07,04,0x09,01,10,14,03,05,12,02,15,0x08,06,
                    01,04,11,13,12,03,07,14,10,15,06,0x08,00,05,0x09,02,
                    06,11,13,0x08,01,04,10,07,0x09,05,00,15,14,02,03,12},
        new byte[] {13,02,0x08,04,06,15,11,01,10,0x09,03,14,05,00,12,07,
                    01,15,13,0x08,10,03,07,04,12,05,06,11,00,14,0x09,02,
                    07,11,04,01,0x09,12,14,02,00,06,10,13,15,03,05,0x08,
                    02,01,14,07,04,10,0x08,13,15,12,0x09,00,03,05,06,11}
    };

    // Key rotation schedule
    private static readonly byte[] Rots = new byte[] {1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,2};

    // Transpose bits according to table
    private static void TransPose(char[] data, byte[] t, int n)
    {
        char[] x = new char[64];
        for (int i = 0; i < 64; i++)
            x[i] = data[i];

        for (int i = 0; i < n; i++)
            data[i] = x[t[i] - 1];
    }

    // Rotate key left
    private static void Rotate(char[] key)
    {
        char[] x = new char[64];
        for (int i = 0; i < 56; i++)
            x[i] = key[i];

        for (int i = 0; i < 55; i++)
            x[i] = x[i + 1];

        x[27] = key[1];
        x[55] = key[28];

        for (int i = 0; i < 56; i++)
            key[i] = x[i];
    }

    // Generate round keys
    private static void SetKey(char[] key, char[][] ckey)
    {
        char[] ikey = new char[64];
        for (int i = 0; i < 64; i++)
            ikey[i] = key[i];

        for (int i = 0; i < 16; i++)
        {
            for (int j = 0; j < Rots[i]; j++)
                Rotate(ikey);

            for (int j = 0; j < 64; j++)
                ckey[i][j] = ikey[j];
            TransPose(ckey[i], KeyTr2, 48);
        }
    }

    // Feistel function
    private static void F(int i, char[] a, char[] x, char[][] ckey)
    {
        char[] e = new char[64];
        char[] ikey = new char[64];
        char[] y = new char[64];

        Array.Copy(a, e, 64);
        TransPose(e, Etr, 48);
        Array.Copy(ckey[i], ikey, 64);

        for (int j = 0; j < 48; j++)
            y[j] = (e[j] + ikey[j] == 1) ? (char)1 : (char)0;

        for (int k = 0; k < 8; k++)
        {
            int r = 32 * y[6 * k + 5] + 16 * y[6 * k] + 8 * y[6 * k + 4] + 4 * y[6 * k + 3] + 2 * y[6 * k + 2] + y[6 * k + 1];
            byte ch = S[k][r];
            x[4 * k] = (ch & 8) != 0 ? (char)1 : (char)0;
            x[4 * k + 1] = (ch & 4) != 0 ? (char)1 : (char)0;
            x[4 * k + 2] = (ch & 2) != 0 ? (char)1 : (char)0;
            x[4 * k + 3] = (ch & 1) != 0 ? (char)1 : (char)0;
        }

        TransPose(x, Ptr, 32);
    }

    // DES encryption (bit level)
    private static void DesInternal(char[] plaintext, char[] key, char[] ciphertext, bool decrypt)
    {
        char[] a = new char[64];
        char[] b = new char[64];
        char[] x = new char[64];
        char[] _key = new char[64];
        char[][] ckey = new char[16][];
        for (int i = 0; i < 16; i++)
            ckey[i] = new char[64];

        Array.Copy(key, _key, 64);
        Array.Copy(plaintext, a, 64);
        TransPose(a, InitialTr, 64);
        TransPose(_key, KeyTr1, 56);
        SetKey(_key, ckey);

        for (int i = 0; i < 16; i++)
        {
            Array.Copy(a, b, 64);
            for (int j = 0; j < 32; j++)
                a[j] = b[j + 32];

            int round = decrypt ? (15 - i) : i;
            F(round, a, x, ckey);

            for (int j = 0; j < 32; j++)
                a[j + 32] = (b[j] + x[j] == 1) ? (char)1 : (char)0;
        }

        TransPose(a, Swap, 64);
        TransPose(a, FinalTr, 64);
        Array.Copy(a, ciphertext, 64);
    }

    // Expand 8 bytes to 64 bits
    private static void ExpandT(byte[] s, char[] d)
    {
        for (int i = 0; i < 8; i++)
        {
            byte ch = s[i];
            byte k = 0x80;
            for (int j = 0; j < 8; j++)
            {
                d[i * 8 + j] = (ch & (k >> j)) != 0 ? (char)1 : (char)0;
            }
        }
    }

    // Pack 64 bits to 8 bytes
    private static void PackingT(char[] s, byte[] d)
    {
        for (int i = 0; i < 8; i++)
        {
            byte k = 0x80;
            byte ch = 0;
            for (int j = 0; j < 8; j++)
            {
                if (s[i * 8 + j] != 0)
                    ch |= (byte)(k >> j);
            }
            d[i] = ch;
        }
    }

    /// <summary>
    /// DES encrypt 8-byte block
    /// </summary>
    public static void Encrypt(byte[] plaintext, byte[] key, byte[] ciphertext)
    {
        char[] pt = new char[64];
        char[] ct = new char[64];
        char[] k = new char[64];

        ExpandT(plaintext, pt);
        ExpandT(key, k);
        DesInternal(pt, k, ct, false);
        PackingT(ct, ciphertext);
    }

    /// <summary>
    /// DES decrypt 8-byte block
    /// </summary>
    public static void Decrypt(byte[] ciphertext, byte[] key, byte[] plaintext)
    {
        char[] pt = new char[64];
        char[] ct = new char[64];
        char[] k = new char[64];

        ExpandT(ciphertext, ct);
        ExpandT(key, k);
        DesInternal(ct, k, pt, true);
        PackingT(pt, plaintext);
    }
}
