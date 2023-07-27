/*
 * Developed by Cain532
 * A great deal of research went into building this, please use it and enjoy it as much as I did making it!
 * I believe in making as many things Open Source as possible, it may not be the best and I'm sure there are improvements to be made, but we can just call this a stepping stone!
 * Most of the research was built from a document released by Kong K Rool and Macrox back around 2003. You can find it on archive.org I'm sure!
 * You will need to make your own reference to ps3tmapi_net, which is a library included with the PS3 SDK
 * (c) 2023 ~ Cain532
 */ 

using System;
using System.Linq;
using System.Net;
using System.Windows.Forms;

namespace GSAPI
{
    public class GameShark
    {
        private uint processID; // used to lock in on the ps1 emu process being used.
        private uint[] processes; // we need to collect all the process ID's before we can go through them.
        private string IP; // This will be the target PS3's IP Address
        /// <summary>
        /// Nifty bit of code I tweaked a bit. Got this from Diitz ages ago as a means to use ProDG on any application to include vsh and in this case ps1! Credit to Diitz!
        /// It's effectively a boolean where if you set it to true, you enable debug options
        /// </summary>
        public bool debug
        {
            get
            {
                if (IP == "") { MessageBox.Show("You need to set an IP!"); return false; }
                else
                    return new WebClient().DownloadString($"http://{IP}/peek.lv2?0x00003B38").Contains("38 60 00 01 4E 80 00 20");
            }
            set
            {
                if (IP != "")
                {
                    string data = value ? "386000014E800020" : "E92280087C0802A6";
                    string magic = $"http://{IP}/poke.lv2?0x00003B38={data}";
                    new WebClient().DownloadString(magic);
                }
            }
        }
        /// <summary>
        /// Generic function from the PS3TMAPI lib to get a region of memory as a byte array
        /// </summary>
        /// <param name="Address">Offset to read from</param>
        /// <param name="Bytes">output bytes we are dumping data to</param>
        private void GetMemory(uint Address, byte[] Bytes) => PS3TMAPI.ProcessGetMemory(0, PS3TMAPI.UnitType.PPU, processID, 0, Address, ref Bytes);
        /// <summary>
        /// Leverages the GetMemory functions, but tunes it down and allows us to grab a smaller array of bytes.
        /// </summary>
        /// <param name="offset">Offset to read from</param>
        /// <param name="length">How far in bytes are we reading?</param>
        /// <returns></returns>
        private byte[] GetBytes(uint offset, int length)
        {
            byte[] buffer = new byte[length];
            GetMemory(offset, buffer);
            return buffer;
        }
        /// <summary>
        /// Using the GetBytes function above, we can grab a 16 bit integer (2 byte length, otherwise known as a short)
        /// </summary>
        /// <param name="offset">Offset to read from</param>
        /// <returns></returns>
        private short ReadInt16(uint offset)
        {
            byte[] buff = GetBytes(offset, 2);
            Array.Reverse(buff, 0, 2);
            return BitConverter.ToInt16(buff, 0);
        }
        /// <summary>
        /// Same concept as reading, but this time we want to write the data.
        /// </summary>
        /// <param name="offset">Offset to write to</param>
        /// <param name="data">data we are writing</param>
        private void WriteInt16(uint offset, short data)
        {
            byte[] buff = new byte[2];
            BitConverter.GetBytes(data).CopyTo(buff, 0);
            Array.Reverse(buff, 0, 2);
            PS3TMAPI.ProcessSetMemory(0, PS3TMAPI.UnitType.PPU, processID, 0, offset, buff);
        }
        /// <summary>
        /// String to Byte Array effectively take a string of hexidecimal code and converts it to a usable byte array
        /// </summary>
        /// <param name="hex">The stirng must be in hexidecimal, otherwise you'll run into errors!</param>
        /// <returns>An array of bytes</returns>
        public static byte[] STBA(string hex)
        {
            return (from x in Enumerable.Range(0, hex.Length)
                    where x % 2 == 0
                    select Convert.ToByte(hex.Substring(x, 2), 16)).ToArray();
        }
        /// <summary>
        /// As soon as this gets called, it will grab the name of the process, provided processID is not null.
        /// </summary>
        private string ProcessName
        {
            get
            {
                PS3TMAPI.ProcessInfo current;
                PS3TMAPI.GetProcessInfo(0, processID, out current);
                return current.Hdr.ELFPath.Split('/').Last().Split('.')[0];
            }
        }
        /// <summary>
        /// This 'magic' number is a Pointer which holds an offset I use to balance out the difference in addresses between a GameShark code and it's actual PS3 memory address. 
        /// Because there are 2 emu's used for the PS1 on PS3, there are 2 possible pointers, and 2 possible mathematic differences.
        /// The Pointer is nested and contains the pointer to the mount location of the PS1 game
        /// </summary>
        private uint magic
        {
            get
            {
                uint address = (uint)(!ProcessName.Contains("net") ? 0x001BBF20 : 0x0017B710);
                byte[] buff = GetBytes(address, 4);
                Array.Reverse(buff, 0, 4);
                return BitConverter.ToUInt32(buff, 0);
            }
        }
        /// <summary>
        /// Writes a single line Gameshark code, breaks down the GameShark string into a usable bit of code that can get passed through with no issues.
        /// The first byte is an identifier which in the past told Gameshark what TYPE of code it was.
        /// The second int16 is the address which needs to be written to
        /// The last int16 is the data being written
        /// </summary>
        /// <param name="input">Single Line GameShark code</param>
        public void Write(string input)
        {
            if (input != "")
            {
                string identifier = input.Remove(2);
                uint address = (Convert.ToUInt32(input.Split(' ')[0].Remove(0, 2), 16) + magic);
                byte[] bytes = STBA(input.Split(' ')[1]);
                short data = (short)(bytes[0] + (bytes[1] << 8));
                if (identifier == "80" || identifier == "30") WriteInt16(address, data); //simple write
                if (identifier == "10" || identifier == "20")
                {
                    short toWrite = (short)(ReadInt16(address) + data);
                    WriteInt16(address, toWrite);
                } //increment value
                if (identifier == "11" || identifier == "21")
                {
                    short toWrite = (short)(ReadInt16(address) - data);
                    WriteInt16(address, toWrite);
                } //decrement value
            }
        }
        /// <summary>
        /// Writes multi-line GameShark codes such as Joker Commands, Serial Repeaters and so on.
        /// </summary>
        /// <param name="input">Multi-Line GameShark code</param>
        public void Write(string[] input)
        {
            if (input.Length > 0)
            {
                try
                {
                    for (int b = 0; b < input.Length; b++)
                    {
                        uint address = (Convert.ToUInt32(input[b].Split(' ')[0].Remove(0, 2), 16) + magic);
                        byte[] bytes = STBA(input[b].Split(' ')[1]);
                        short data = (short)(bytes[0] + (bytes[1] << 8));
                        string identifier = input[0].Remove(2);
                        if (identifier == "80" || identifier == "30") WriteInt16(address, data); //simple write
                        if (identifier == "10" || identifier == "20")
                        {
                            short toWrite = (short)(ReadInt16(address) + data);
                            WriteInt16(address, toWrite);
                        } //increment value
                        if (identifier == "11" || identifier == "21")
                        {
                            short toWrite = (short)(ReadInt16(address) - data);
                            WriteInt16(address, toWrite);
                        } //decrement value
                        if (identifier == "E0" || identifier == "D0")
                        {
                            if (ReadInt16(address) == data)
                            {
                                for (int i = 1; i < input.Length; i++)
                                    Write(input[i]);
                            }
                        } //Equal To
                        if (identifier == "E1" || identifier == "D1")
                        {
                            if (ReadInt16(address) != data)
                            {
                                for (int i = 1; i < input.Length; i++)
                                    Write(input[i]);
                            }
                        } //Different To
                        if (identifier == "E2" || identifier == "D2")
                        {
                            if (ReadInt16(address) < data)
                            {
                                for (int i = 1; i < input.Length; i++)
                                    Write(input[i]);
                            }
                        } //Less Than
                        if (identifier == "E3" || identifier == "D3")
                        {
                            if (ReadInt16(address) > data)
                            {
                                for (int i = 1; i < input.Length; i++)
                                    Write(input[i]);
                            }
                        } //Greater Than
                        if (identifier == "50")
                        {
                            int amnt = Convert.ToInt32(input[b].Split(' ')[0].Remove(0, 4).Remove(2), 16);
                            int jump = Convert.ToInt32(input[b].Split(' ')[0].Remove(0, 4).Remove(0, 2), 16);
                            uint _address = (Convert.ToUInt32(input[b + 1].Split(' ')[0].Remove(0, 2), 16) + magic);
                            byte[] _bytes = STBA(input[b + 1].Split(' ')[1]);
                            for (int a = 0; a < amnt; a++)
                            {
                                byte[] shit = BitConverter.GetBytes(_bytes[1] += bytes[1]);
                                short toWrite = (short)(_bytes[0] + (shit[0] << 8)); //line 2!!!
                                WriteInt16((uint)(_address + (a * jump)), toWrite);
                            }
                            b++;
                        } //Serial Repeater (For Loop)
                    }
                }
                catch (Exception ex) { Console.WriteLine(ex.Message); }
            }
        }
        /// <summary>
        /// Simple Connection dialog to connect and attach to any process ID identifiable as a PS1 emu
        /// </summary>
        /// <returns>Returns a successful, or unsuccessful attempt</returns>
        public bool ConnectToPSX()
        {
            if (PS3TMAPI.SUCCEEDED(PS3TMAPI.InitTargetComms()))
            {
                if (PS3TMAPI.SUCCEEDED(PS3TMAPI.Connect(0, null)))
                {
                    PS3TMAPI.GetProcessList(0, out processes);
                    PS3TMAPI.ProcessInfo pInfo;
                    for (int i = 0; i < processes.Length; i++)
                    {
                        PS3TMAPI.GetProcessInfo(0, processes[i], out pInfo);
                        if (pInfo.Hdr.ELFPath != null && pInfo.Hdr.ELFPath.Contains("ps1"))
                        {
                            PS3TMAPI.ProcessAttach(0, PS3TMAPI.UnitType.PPU, processes[i]);
                            PS3TMAPI.ProcessContinue(0, processes[i]);
                            processID = processes[i];
                            return true;
                        }
                    }
                    return false;
                }
                return false;
            }
            return false;
        }
        /// <summary>
        /// Establishing the API
        /// </summary>
        /// <param name="IP">Playstation 3's IP address</param>
        /// <param name="ProcessID">Process ID can be inputted manually</param>

        public GameShark(string IP, uint ProcessID) {
            this.IP = IP;
            debug = true;
            this.processID = ProcessID;
        }
        /// <summary>
        /// Establishing the API
        /// </summary>
        /// <param name="IP">Playstation 3's IP Address</param>
        public GameShark(string IP) {
            this.IP = IP;
            debug = true;
        }
    }

}
