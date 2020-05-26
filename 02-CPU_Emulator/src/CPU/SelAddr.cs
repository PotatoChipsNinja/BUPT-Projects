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
    public partial class SelAddr : Form
    {
        public SelAddr()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (textBox1.Text != "")
            {
                MainForm.that.Paddr = Convert.ToInt32(textBox1.Text.Substring(0, textBox1.Text.Length - 1), 16);
                Close();
            }
        }
    }
}
