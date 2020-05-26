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
    public partial class EditDialog : Form
    {
        public EditDialog()
        {
            InitializeComponent();
        }

        private void buttonOK_Click(object sender, EventArgs e)
        {
            if (textBoxVal.Text != "")
            {
                MainForm.that.Edit_RAM(Convert.ToInt32(textBoxAddr.Text.Substring(0, textBoxAddr.Text.Length - 1), 16), textBoxVal.Text, true);
                Close();
            }
        }

        private void buttonCancel_Click(object sender, EventArgs e)
        {
            Close();
        }
    }
}
