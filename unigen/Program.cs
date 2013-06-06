using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Globalization;

namespace unigen
{
    class Program
    {
        static void Main(string[] args)
        {
            /*Console.WriteLine("Input File:");
            string inf = Console.ReadLine();
            Console.WriteLine("Output File:");
            string outf = Console.ReadLine(); */
            process();
        }
        static void process()
        {
            StreamReader input = new StreamReader("UnicodeData.txt");
            ushort startdef = 0x0019;
            ushort enddef = 0xFFFD;
            StringBuilder rcfile = new StringBuilder();
            string line;
            TextInfo casefix = new CultureInfo("en-US", false).TextInfo;
            while ((line = input.ReadLine()) != null)
            {
                StringBuilder templine = new StringBuilder();
                string[] items = line.Split(';');
                if (items[0] == "10000")
                    break;
                ushort ucode = ushort.Parse(items[0], System.Globalization.NumberStyles.HexNumber);
                if (ucode > startdef && ucode < enddef)
                    
                    templine.Append(ucode.ToString() + " \"" + items[1] + "\" \n");
                    rcfile.Append(casefix.ToTitleCase(templine.ToString().ToLower()));
                    if (ucode == enddef)
                        break;
                
                
            }
            using (StreamWriter output = new StreamWriter("res.txt"))
            {
                output.Write(rcfile.ToString());
            }
        }


            


        }
    }

