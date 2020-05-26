using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace CPU
{
    public partial class InputStr : Form
    {
        public InputStr()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (textBox1.Text != "" && textBox2.Text != "")
            {
                MainForm.that.arr_reg[0] = Convert.ToInt32(textBox2.Text.Substring(0, textBox2.Text.Length - 1), 16);
                MainForm.that.arr_reg[1] = Convert.ToInt32(textBox2.Text.Substring(0, textBox2.Text.Length - 1), 16) + textBox1.Text.Length - 1;
                for (int i = 0; i < textBox1.Text.Length; ++i)
                {
                    MainForm.that.arr_content[MainForm.that.arr_reg[0] + i] = Convert.ToString(MainForm.that.DecToBin((int)textBox1.Text[i], 8));
                    MainForm.that.arr_symbol[MainForm.that.arr_reg[0] + i] = Convert.ToString(textBox1.Text[i]);
                    MainForm.that.Draw_RAM();
                }
                Close();
            }
        }
    }
}
