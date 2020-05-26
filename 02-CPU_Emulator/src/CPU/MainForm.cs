using System;
using System.IO;
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
    public partial class MainForm : Form
    {
        public string[] arr_symbol = new string[256];
        public string[] arr_content = new string[256];

        private int pc_dec = 0, ar_dec, dr_dec, ir_addr, x, y, pause_addr;
        public int[] arr_reg = new int[4];
        private bool z, pause_signal, pause_allow, auto_run = false;

        public static MainForm that;
        public int Paddr;       // 加载程序的首地址

        public MainForm()
        {
            InitializeComponent();
            that = this;
        }

        private void button_exit_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void button_about_Click(object sender, EventArgs e)
        {
            MessageBox.Show("CPU Emulator\r\n\r\nPowered by Shi Yuhui\r\nCopyright © 2019 All Rights Reserved.", "关于");
        }

        private void button_open_Click(object sender, EventArgs e)
        {
            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                new SelAddr().ShowDialog();
                using (StreamReader reader = new StreamReader(openFileDialog.OpenFile()))
                {
                    string line;
                    int i = Paddr;

                    //Init_RAM();
                    while ((line = reader.ReadLine()) != null)
                    {
                        arr_symbol[i] = line;
                        arr_content[i++] = Assemble(line);
                    }

                    Init_Reg();
                    Draw_CPU();
                    Draw_RAM();

                    reader.Close();
                }
            }
        }

        private void MainForm_Load(object sender, EventArgs e)
        {
            openFileDialog.InitialDirectory = System.Environment.CurrentDirectory;

            Init_RAM();
            Draw_RAM();

            Init_Reg();
            Draw_CPU();
        }

        private void Draw_CPU()
        {
            textBox_PC.Text = DecToBin(pc_dec, 8) + " (" + pc_dec.ToString("X").PadLeft(2, '0') + "H)";
            textBox_AR.Text = DecToBin(ar_dec, 8) + " (" + ar_dec.ToString("X").PadLeft(2, '0') + "H)";
            textBox_DR.Text = DecToBin(dr_dec, 8) + " (" + dr_dec.ToString("X").PadLeft(2, '0') + "H)";

            if (ir_addr == -1)
                textBox_IR.Text = "00000000 (NOP)";
            else
                textBox_IR.Text = arr_content[ir_addr] + " (" + arr_symbol[ir_addr] + ")";

            textBox_R0.Text = DecToBin(arr_reg[0], 8) + " (" + arr_reg[0].ToString("X").PadLeft(2, '0') + "H)";
            textBox_R1.Text = DecToBin(arr_reg[1], 8) + " (" + arr_reg[1].ToString("X").PadLeft(2, '0') + "H)";
            textBox_R2.Text = DecToBin(arr_reg[2], 8) + " (" + arr_reg[2].ToString("X").PadLeft(2, '0') + "H)";
            textBox_R3.Text = DecToBin(arr_reg[3], 8) + " (" + arr_reg[3].ToString("X").PadLeft(2, '0') + "H)";
        }

        public void Draw_RAM()
        {
            listView1.BeginUpdate();
            listView1.Items.Clear();
            for (int i = 0; i < 256; ++i)
            {
                ListViewItem lvi = new ListViewItem();
                lvi.Text = i.ToString("X").PadLeft(2, '0') + "H";
                lvi.SubItems.Add(arr_symbol[i]);
                lvi.SubItems.Add(arr_content[i]);
                listView1.Items.Add(lvi);
                lvi.Selected = (i == 0 && ir_addr == -1) || (i == ir_addr);
            }
            listView1.EndUpdate();
            listView1.Focus();
        }

        private void Init_RAM()
        {
            for (int i = 0; i < 256; ++i)
            {
                arr_symbol[i] = "";
                arr_content[i] = "00000000";
            }
        }

        private string Encode_Reg(string str)
        {
            // 对寄存器名进行编码，从"00"到"11"
            string ans = "";
            switch (str)
            {
                case "R0":
                    ans = "00";
                    break;
                case "R1":
                    ans = "01";
                    break;
                case "R2":
                    ans = "10";
                    break;
                case "R3":
                    ans = "11";
                    break;
            }
            return ans;
        }

        private string HexToBin(string binstr)
        {
            int val = Convert.ToInt32(binstr, 16);
            return Convert.ToString(val, 2);
        }

        private void button_step_Click(object sender, EventArgs e)
        {
            Execute();
            CheckPause();
        }

        private void button_auto_Click(object sender, EventArgs e)
        {
            auto_run = true;
        }

        private void Execute()
        {
            ar_dec = pc_dec;
            dr_dec = Convert.ToInt32(arr_content[ar_dec], 2);
            ir_addr = ar_dec;

            string[] parts = arr_symbol[ir_addr].Split(' ');
            switch (parts[0])
            {
                /*
                case "NOP":
                    pc_dec++;
                    break;
                */
                case "ADD":
                    y = arr_reg[Convert.ToInt32(parts[2].Substring(1, 1))];
                    x = arr_reg[Convert.ToInt32(parts[1].Substring(1, 1))];
                    arr_reg[Convert.ToInt32(parts[1].Substring(1, 1))] = x + y;
                    z = (arr_reg[Convert.ToInt32(parts[1].Substring(1, 1))] == 0);
                    pc_dec++;
                    break;
                case "SUB":
                    y = arr_reg[Convert.ToInt32(parts[2].Substring(1, 1))];
                    x = arr_reg[Convert.ToInt32(parts[1].Substring(1, 1))];
                    arr_reg[Convert.ToInt32(parts[1].Substring(1, 1))] = x - y;
                    z = (arr_reg[Convert.ToInt32(parts[1].Substring(1, 1))] == 0);
                    pc_dec++;
                    break;
                case "INC":
                    y = 1;
                    x = arr_reg[Convert.ToInt32(parts[1].Substring(1, 1))];
                    arr_reg[Convert.ToInt32(parts[1].Substring(1, 1))] = x + y;
                    z = (arr_reg[Convert.ToInt32(parts[1].Substring(1, 1))] == 0);
                    pc_dec++;
                    break;
                case "DEC":
                    y = 1;
                    x = arr_reg[Convert.ToInt32(parts[1].Substring(1, 1))];
                    arr_reg[Convert.ToInt32(parts[1].Substring(1, 1))] = x - y;
                    z = (arr_reg[Convert.ToInt32(parts[1].Substring(1, 1))] == 0);
                    pc_dec++;
                    break;
                case "LAD":
                    ar_dec = arr_reg[Convert.ToInt32(parts[2].Substring(2, 1))];
                    dr_dec = Convert.ToInt32(arr_content[ar_dec], 2);
                    arr_reg[Convert.ToInt32(parts[1].Substring(1, 1))] = dr_dec;
                    pc_dec++;
                    break;
                case "STO":
                    ar_dec = arr_reg[Convert.ToInt32(parts[2].Substring(2, 1))];
                    dr_dec = arr_reg[Convert.ToInt32(parts[1].Substring(1, 1))];
                    arr_content[ar_dec] = DecToBin(dr_dec, 8);
                    arr_symbol[ar_dec] = Convert.ToString((char)dr_dec);
                    Draw_RAM();
                    pc_dec++;
                    break;
                /*
                case "EDA":
                    arr_reg[0] = Convert.ToInt32(parts[1].Substring(0, 2), 16);
                    pc_dec++;
                    break;
                case "EDB":
                    arr_reg[1] = Convert.ToInt32(parts[1].Substring(0, 2), 16);
                    pc_dec++;
                    break;
                */
                case "MOV":
                    arr_reg[Convert.ToInt32(parts[1].Substring(1, 1))] = arr_reg[Convert.ToInt32(parts[2].Substring(1, 1))];
                    pc_dec++;
                    break;
                case "JPZ":
                    pc_dec++;
                    if (z)
                        pc_dec += Convert.ToInt32(parts[1].Substring(0, 2), 16);
                    break;
                case "JMP":
                    pc_dec++;
                    pc_dec += Convert.ToInt32(parts[1].Substring(0, 2), 16);
                    pc_dec = Convert.ToInt32(DecToBin(pc_dec, 8), 2);
                    break;
                case "CLA":
                    for (int i = 0; i < 4; ++i)
                        arr_reg[i] = 0;
                    pc_dec++;
                    break;
                case "STP":
                    Draw_RAM();
                    MessageBox.Show("程序运行结束");
                    pc_dec++;
                    break;
                case "EIT":
                    pause_allow = true;
                    label9.Text = "允许中断：是";
                    button_pause.Enabled = true;
                    pc_dec++;
                    break;
                case "DIT":
                    pause_allow = false;
                    label9.Text = "允许中断：否  断点PC：" + pause_addr.ToString("X").PadLeft(2, '0') + "H";
                    button_pause.Enabled = false;
                    pc_dec++;
                    break;
                case "RET":
                    pc_dec = pause_addr;
                    break;
                default:
                    pc_dec++;
                    break;
            }
            Draw_CPU();
            Draw_RAM();
        }

        private bool CheckPause()
        {
            if(pause_allow && pause_signal)
            {
                pause_signal = false;
                pause_addr = pc_dec;
                pc_dec = Convert.ToInt32(textBox_INT_addr.Text.Substring(0, textBox_INT_addr.Text.Length - 1), 16);
                Draw_CPU();
                return true;
            }
            return false;
        }

        public void Edit_RAM(int addr, string str, bool type)
        {
            // type为真则str为标识符，type为假则str为二进制内容
            if (type)
            {
                arr_symbol[addr] = str;
                arr_content[addr] = Assemble(str);
            }
            else
            {
                arr_content[addr] = str;
                arr_symbol[addr] = Convert.ToInt32(str, 2).ToString("X").PadLeft(2, '0') + "H";
            }
            Draw_RAM();
        }

        private void button_CLRAM_Click(object sender, EventArgs e)
        {
            Init_RAM();
            Draw_RAM();
        }

        private void buttonEditReg_Click(object sender, EventArgs e)
        {
            textBox_PC.ReadOnly = false;
            textBox_R0.ReadOnly = false;
            textBox_R1.ReadOnly = false;
            textBox_R2.ReadOnly = false;
            textBox_R3.ReadOnly = false;

            textBox_PC.Text = DecToBin(pc_dec, 8);
            textBox_R0.Text = DecToBin(arr_reg[0], 8);
            textBox_R1.Text = DecToBin(arr_reg[1], 8);
            textBox_R2.Text = DecToBin(arr_reg[2], 8);
            textBox_R3.Text = DecToBin(arr_reg[3], 8);

            buttonEditReg.Visible = false;
            buttonFinishEdit.Visible = true;
            button_step.Enabled = false;
            button_auto.Enabled = false;
            button_CLREG.Enabled = false;
            buttonFinishEdit.Focus();
        }

        private void buttonFinishEdit_Click(object sender, EventArgs e)
        {
            pc_dec = Convert.ToInt32(textBox_PC.Text, 2);
            arr_reg[0] = Convert.ToInt32(textBox_R0.Text, 2);
            arr_reg[1] = Convert.ToInt32(textBox_R1.Text, 2);
            arr_reg[2] = Convert.ToInt32(textBox_R2.Text, 2);
            arr_reg[3] = Convert.ToInt32(textBox_R3.Text, 2);

            textBox_PC.Text = DecToBin(pc_dec, 8) + " (" + pc_dec.ToString("X").PadLeft(2, '0') + "H)";
            textBox_R0.Text = DecToBin(arr_reg[0], 8) + " (" + arr_reg[0].ToString("X").PadLeft(2, '0') + "H)";
            textBox_R1.Text = DecToBin(arr_reg[1], 8) + " (" + arr_reg[1].ToString("X").PadLeft(2, '0') + "H)";
            textBox_R2.Text = DecToBin(arr_reg[2], 8) + " (" + arr_reg[2].ToString("X").PadLeft(2, '0') + "H)";
            textBox_R3.Text = DecToBin(arr_reg[3], 8) + " (" + arr_reg[3].ToString("X").PadLeft(2, '0') + "H)";

            buttonEditReg.Visible = true;
            buttonFinishEdit.Visible = false;
            button_step.Enabled = true;
            button_auto.Enabled = true;
            button_CLREG.Enabled = true;
        }

        private void textBox_PC_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar != '0' && e.KeyChar != '1' && e.KeyChar != '\b')
                e.Handled = true;
        }

        private void textBox_R0_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar != '0' && e.KeyChar != '1' && e.KeyChar != '\b')
                e.Handled = true;
        }

        private void textBox_R1_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar != '0' && e.KeyChar != '1' && e.KeyChar != '\b')
                e.Handled = true;
        }

        private void button_pause_MouseDown(object sender, MouseEventArgs e)
        {
            pause_signal = true;
            new InputStr().ShowDialog();
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            if (auto_run)
            {
                if (ir_addr == -1 || arr_symbol[ir_addr] != "STP")
                {
                    Execute();
                    if (CheckPause())
                        auto_run = false;
                }
                else
                    auto_run = false;
            }
        }

        private void textBox_R2_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar != '0' && e.KeyChar != '1' && e.KeyChar != '\b')
                e.Handled = true;
        }

        private void textBox_R3_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar != '0' && e.KeyChar != '1' && e.KeyChar != '\b')
                e.Handled = true;
        }

        private void button_CLREG_Click(object sender, EventArgs e)
        {
            Init_Reg();
            Draw_CPU();
        }

        private void button_EditRAM_Click(object sender, EventArgs e)
        {
            new EditDialog().ShowDialog();
        }

        public string DecToBin(int val, int bits)
        {
            string str = Convert.ToString(val, 2).PadLeft(bits, '0');
            return str.Substring(str.Length - bits);
        }

        private string Assemble(string symbol)
        {
            string content = "";
            string[] parts = symbol.Split(' ');
            switch (parts[0])
            {
                case "NOP":
                    content = "00000000";
                    break;
                case "ADD":
                    content = "0001" + Encode_Reg(parts[1].Substring(0, 2)) + Encode_Reg(parts[2]);
                    break;
                case "SUB":
                    content = "0010" + Encode_Reg(parts[1].Substring(0, 2)) + Encode_Reg(parts[2]);
                    break;
                case "INC":
                    content = "0011" + Encode_Reg(parts[1]) + "00";
                    break;
                case "DEC":
                    content = "1011" + Encode_Reg(parts[1]) + "00";
                    break;
                case "LAD":
                    content = "0100" + Encode_Reg(parts[1].Substring(0, 2)) + Encode_Reg(parts[2].Substring(1, 2));
                    break;
                case "STO":
                    content = "0101" + Encode_Reg(parts[2].Substring(1, 2)) + Encode_Reg(parts[1].Substring(0, 2));
                    break;
                /*
                case "EDA":
                    content = "0110" + HexToBin(parts[1].Substring(0, 2)).PadLeft(4, '0');
                    break;
                case "EDB":
                    content = "0111" + HexToBin(parts[1].Substring(0, 2)).PadLeft(4, '0');
                    break;
                */
                case "MOV":
                    content = "1000" + Encode_Reg(parts[1].Substring(0, 2)) + Encode_Reg(parts[2]);
                    break;
                case "JPZ":
                    content = "1001" + HexToBin(parts[1].Substring(0, 2)).PadLeft(4, '0');
                    break;
                case "JMP":
                    content = "1010" + HexToBin(parts[1].Substring(0, 2)).PadLeft(4, '0');
                    break;
                case "CLA":
                    content = "11100000";
                    break;
                case "STP":
                    content = "11110000";
                    break;
                case "EIT":
                    content = "01100000";
                    break;
                case "DIT":
                    content = "01110000";
                    break;
                case "RET":
                    content = "11000000";
                    break;
                default:
                    content = HexToBin(parts[0].Substring(0, 2)).PadLeft(8, '0');
                    break;
            }
            return content;
        }

        private void Init_Reg()
        {
            pc_dec = ar_dec = dr_dec = 0;
            ir_addr = -1;

            z = false;
            pause_signal = false;
            pause_allow = true;

            for (int i = 0; i < 4; ++i)
                arr_reg[i] = 0;
        }
    }
}
