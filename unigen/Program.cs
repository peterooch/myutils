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
            Console.WriteLine("Input File:(\"UnicodeData.txt\")");
            string ifile = Console.ReadLine();
            if (ifile == "")
                ifile = "UnicodeData.txt";
            Console.WriteLine("Output File:(\"result.txt\")");
            string ofile = Console.ReadLine();
            if (ofile == "")
                ofile = "result.txt";
            process(ifile,ofile);
        }
        static void process(string infile,string outfile)
        {
            StreamReader input = new StreamReader(infile);
            ushort startdef = 0x001F;
            ushort enddef = 0xFFFC;
            List<ushort> exp = new List<ushort>();
            exp.Add(0x0080);
            exp.Add(0x0081);
            exp.Add(0x0084);
            exp.Add(0x0099);
            StringBuilder rcfile = new StringBuilder();            
            string line;
            TextInfo casefix = new CultureInfo("en-US", false).TextInfo;
            while ((line = input.ReadLine()) != null)
            {
                StringBuilder templine = new StringBuilder();
                int i = 1;
                string[] items = line.Split(';');
                ushort ucode = ushort.Parse(items[0], System.Globalization.NumberStyles.HexNumber);
                if (ucode <= startdef || (ucode < 0x00A0 && ucode > 0x007E))
                    i = 10;
                if (exp.Contains(ucode))
                    continue;
                templine.Append(ucode.ToString() + " \"" + items[i] + "\" \n");
                rcfile.Append(casefix.ToTitleCase(templine.ToString().ToLower()));
                if (ucode == enddef)
                    break;    
            }
            using (StreamWriter output = new StreamWriter(outfile))
            {   
                //finishing touches
                rcfile.Replace("Cjk", "CJK");
                //specifics
                rcfile.Replace(" (Nel)", "").Replace(" (Cr)", "").Replace(" (Ff)", "").Replace(" (Lf)", "");
                output.Write(rcfile.ToString());
            }
        }
    }
}

