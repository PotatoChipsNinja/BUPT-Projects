namespace CPU
{
    partial class MainForm
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要修改
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.group_RAM = new System.Windows.Forms.GroupBox();
            this.listView1 = new System.Windows.Forms.ListView();
            this.address = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.symbol = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.content = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.button_exit = new System.Windows.Forms.Button();
            this.button_about = new System.Windows.Forms.Button();
            this.group_CPU = new System.Windows.Forms.GroupBox();
            this.textBox_INT_addr = new System.Windows.Forms.TextBox();
            this.label11 = new System.Windows.Forms.Label();
            this.label9 = new System.Windows.Forms.Label();
            this.textBox_R3 = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.textBox_DR = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.textBox_R2 = new System.Windows.Forms.TextBox();
            this.label7 = new System.Windows.Forms.Label();
            this.textBox_AR = new System.Windows.Forms.TextBox();
            this.label8 = new System.Windows.Forms.Label();
            this.textBox_R1 = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.textBox_IR = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.textBox_R0 = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.textBox_PC = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.button_step = new System.Windows.Forms.Button();
            this.button_auto = new System.Windows.Forms.Button();
            this.button_open = new System.Windows.Forms.Button();
            this.group_run = new System.Windows.Forms.GroupBox();
            this.button_pause = new System.Windows.Forms.Button();
            this.openFileDialog = new System.Windows.Forms.OpenFileDialog();
            this.group_edit = new System.Windows.Forms.GroupBox();
            this.buttonEditReg = new System.Windows.Forms.Button();
            this.buttonFinishEdit = new System.Windows.Forms.Button();
            this.button_CLREG = new System.Windows.Forms.Button();
            this.button_CLRAM = new System.Windows.Forms.Button();
            this.button_EditRAM = new System.Windows.Forms.Button();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.group_RAM.SuspendLayout();
            this.group_CPU.SuspendLayout();
            this.group_run.SuspendLayout();
            this.group_edit.SuspendLayout();
            this.SuspendLayout();
            // 
            // group_RAM
            // 
            this.group_RAM.Controls.Add(this.listView1);
            this.group_RAM.Location = new System.Drawing.Point(12, 13);
            this.group_RAM.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.group_RAM.Name = "group_RAM";
            this.group_RAM.Padding = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.group_RAM.Size = new System.Drawing.Size(365, 569);
            this.group_RAM.TabIndex = 0;
            this.group_RAM.TabStop = false;
            this.group_RAM.Text = "RAM";
            // 
            // listView1
            // 
            this.listView1.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.address,
            this.symbol,
            this.content});
            this.listView1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.listView1.FullRowSelect = true;
            this.listView1.GridLines = true;
            this.listView1.HideSelection = false;
            this.listView1.Location = new System.Drawing.Point(3, 28);
            this.listView1.MultiSelect = false;
            this.listView1.Name = "listView1";
            this.listView1.Size = new System.Drawing.Size(359, 537);
            this.listView1.TabIndex = 0;
            this.listView1.UseCompatibleStateImageBehavior = false;
            this.listView1.View = System.Windows.Forms.View.Details;
            // 
            // address
            // 
            this.address.Text = "地址";
            // 
            // symbol
            // 
            this.symbol.Text = "助记符";
            this.symbol.Width = 140;
            // 
            // content
            // 
            this.content.Text = "内容";
            this.content.Width = 120;
            // 
            // button_exit
            // 
            this.button_exit.Location = new System.Drawing.Point(863, 531);
            this.button_exit.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.button_exit.Name = "button_exit";
            this.button_exit.Size = new System.Drawing.Size(82, 50);
            this.button_exit.TabIndex = 1;
            this.button_exit.Text = "退出";
            this.button_exit.UseVisualStyleBackColor = true;
            this.button_exit.Click += new System.EventHandler(this.button_exit_Click);
            // 
            // button_about
            // 
            this.button_about.Location = new System.Drawing.Point(775, 531);
            this.button_about.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.button_about.Name = "button_about";
            this.button_about.Size = new System.Drawing.Size(82, 50);
            this.button_about.TabIndex = 2;
            this.button_about.Text = "关于";
            this.button_about.UseVisualStyleBackColor = true;
            this.button_about.Click += new System.EventHandler(this.button_about_Click);
            // 
            // group_CPU
            // 
            this.group_CPU.Controls.Add(this.textBox_INT_addr);
            this.group_CPU.Controls.Add(this.label11);
            this.group_CPU.Controls.Add(this.label9);
            this.group_CPU.Controls.Add(this.textBox_R3);
            this.group_CPU.Controls.Add(this.label5);
            this.group_CPU.Controls.Add(this.textBox_DR);
            this.group_CPU.Controls.Add(this.label6);
            this.group_CPU.Controls.Add(this.textBox_R2);
            this.group_CPU.Controls.Add(this.label7);
            this.group_CPU.Controls.Add(this.textBox_AR);
            this.group_CPU.Controls.Add(this.label8);
            this.group_CPU.Controls.Add(this.textBox_R1);
            this.group_CPU.Controls.Add(this.label4);
            this.group_CPU.Controls.Add(this.textBox_IR);
            this.group_CPU.Controls.Add(this.label3);
            this.group_CPU.Controls.Add(this.textBox_R0);
            this.group_CPU.Controls.Add(this.label2);
            this.group_CPU.Controls.Add(this.textBox_PC);
            this.group_CPU.Controls.Add(this.label1);
            this.group_CPU.Location = new System.Drawing.Point(394, 13);
            this.group_CPU.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.group_CPU.Name = "group_CPU";
            this.group_CPU.Padding = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.group_CPU.Size = new System.Drawing.Size(551, 225);
            this.group_CPU.TabIndex = 3;
            this.group_CPU.TabStop = false;
            this.group_CPU.Text = "CPU";
            // 
            // textBox_INT_addr
            // 
            this.textBox_INT_addr.Location = new System.Drawing.Point(183, 178);
            this.textBox_INT_addr.Name = "textBox_INT_addr";
            this.textBox_INT_addr.Size = new System.Drawing.Size(77, 31);
            this.textBox_INT_addr.TabIndex = 19;
            this.textBox_INT_addr.Text = "00H";
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(21, 180);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(164, 25);
            this.label11.TabIndex = 18;
            this.label11.Text = "中断服务程序地址";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(288, 180);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(126, 25);
            this.label9.TabIndex = 16;
            this.label9.Text = "允许中断：是";
            // 
            // textBox_R3
            // 
            this.textBox_R3.Location = new System.Drawing.Point(325, 139);
            this.textBox_R3.Name = "textBox_R3";
            this.textBox_R3.ReadOnly = true;
            this.textBox_R3.Size = new System.Drawing.Size(200, 31);
            this.textBox_R3.TabIndex = 15;
            this.textBox_R3.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.textBox_R3_KeyPress);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(286, 142);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(33, 25);
            this.label5.TabIndex = 14;
            this.label5.Text = "R3";
            // 
            // textBox_DR
            // 
            this.textBox_DR.Location = new System.Drawing.Point(60, 139);
            this.textBox_DR.Name = "textBox_DR";
            this.textBox_DR.ReadOnly = true;
            this.textBox_DR.Size = new System.Drawing.Size(200, 31);
            this.textBox_DR.TabIndex = 13;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(21, 142);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(36, 25);
            this.label6.TabIndex = 12;
            this.label6.Text = "DR";
            // 
            // textBox_R2
            // 
            this.textBox_R2.Location = new System.Drawing.Point(325, 102);
            this.textBox_R2.Name = "textBox_R2";
            this.textBox_R2.ReadOnly = true;
            this.textBox_R2.Size = new System.Drawing.Size(200, 31);
            this.textBox_R2.TabIndex = 11;
            this.textBox_R2.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.textBox_R2_KeyPress);
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(286, 105);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(33, 25);
            this.label7.TabIndex = 10;
            this.label7.Text = "R2";
            // 
            // textBox_AR
            // 
            this.textBox_AR.Location = new System.Drawing.Point(60, 102);
            this.textBox_AR.Name = "textBox_AR";
            this.textBox_AR.ReadOnly = true;
            this.textBox_AR.Size = new System.Drawing.Size(200, 31);
            this.textBox_AR.TabIndex = 9;
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(21, 105);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(35, 25);
            this.label8.TabIndex = 8;
            this.label8.Text = "AR";
            // 
            // textBox_R1
            // 
            this.textBox_R1.Location = new System.Drawing.Point(325, 65);
            this.textBox_R1.Name = "textBox_R1";
            this.textBox_R1.ReadOnly = true;
            this.textBox_R1.Size = new System.Drawing.Size(200, 31);
            this.textBox_R1.TabIndex = 7;
            this.textBox_R1.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.textBox_R1_KeyPress);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(286, 68);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(33, 25);
            this.label4.TabIndex = 6;
            this.label4.Text = "R1";
            // 
            // textBox_IR
            // 
            this.textBox_IR.Location = new System.Drawing.Point(60, 65);
            this.textBox_IR.Name = "textBox_IR";
            this.textBox_IR.ReadOnly = true;
            this.textBox_IR.Size = new System.Drawing.Size(200, 31);
            this.textBox_IR.TabIndex = 5;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(21, 68);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(28, 25);
            this.label3.TabIndex = 4;
            this.label3.Text = "IR";
            // 
            // textBox_R0
            // 
            this.textBox_R0.Location = new System.Drawing.Point(325, 28);
            this.textBox_R0.Name = "textBox_R0";
            this.textBox_R0.ReadOnly = true;
            this.textBox_R0.Size = new System.Drawing.Size(200, 31);
            this.textBox_R0.TabIndex = 3;
            this.textBox_R0.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.textBox_R0_KeyPress);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(286, 31);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(33, 25);
            this.label2.TabIndex = 2;
            this.label2.Text = "R0";
            // 
            // textBox_PC
            // 
            this.textBox_PC.Location = new System.Drawing.Point(60, 28);
            this.textBox_PC.Name = "textBox_PC";
            this.textBox_PC.ReadOnly = true;
            this.textBox_PC.Size = new System.Drawing.Size(200, 31);
            this.textBox_PC.TabIndex = 1;
            this.textBox_PC.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.textBox_PC_KeyPress);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(21, 31);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(33, 25);
            this.label1.TabIndex = 0;
            this.label1.Text = "PC";
            // 
            // button_step
            // 
            this.button_step.Location = new System.Drawing.Point(21, 32);
            this.button_step.Name = "button_step";
            this.button_step.Size = new System.Drawing.Size(161, 50);
            this.button_step.TabIndex = 4;
            this.button_step.Text = "单步运行";
            this.button_step.UseVisualStyleBackColor = true;
            this.button_step.Click += new System.EventHandler(this.button_step_Click);
            // 
            // button_auto
            // 
            this.button_auto.Location = new System.Drawing.Point(198, 32);
            this.button_auto.Name = "button_auto";
            this.button_auto.Size = new System.Drawing.Size(161, 50);
            this.button_auto.TabIndex = 5;
            this.button_auto.Text = "连续运行";
            this.button_auto.UseVisualStyleBackColor = true;
            this.button_auto.Click += new System.EventHandler(this.button_auto_Click);
            // 
            // button_open
            // 
            this.button_open.Location = new System.Drawing.Point(687, 532);
            this.button_open.Name = "button_open";
            this.button_open.Size = new System.Drawing.Size(82, 50);
            this.button_open.TabIndex = 6;
            this.button_open.Text = "打开";
            this.button_open.UseVisualStyleBackColor = true;
            this.button_open.Click += new System.EventHandler(this.button_open_Click);
            // 
            // group_run
            // 
            this.group_run.Controls.Add(this.button_pause);
            this.group_run.Controls.Add(this.button_step);
            this.group_run.Controls.Add(this.button_auto);
            this.group_run.Location = new System.Drawing.Point(394, 423);
            this.group_run.Name = "group_run";
            this.group_run.Size = new System.Drawing.Size(551, 101);
            this.group_run.TabIndex = 7;
            this.group_run.TabStop = false;
            this.group_run.Text = "运行";
            // 
            // button_pause
            // 
            this.button_pause.Location = new System.Drawing.Point(379, 32);
            this.button_pause.Name = "button_pause";
            this.button_pause.Size = new System.Drawing.Size(161, 50);
            this.button_pause.TabIndex = 6;
            this.button_pause.Text = "中断";
            this.button_pause.UseVisualStyleBackColor = true;
            this.button_pause.MouseDown += new System.Windows.Forms.MouseEventHandler(this.button_pause_MouseDown);
            // 
            // openFileDialog
            // 
            this.openFileDialog.Filter = "Program files(*.asm;*.txt)|*.asm;*.txt|All files(*.*)|*.*";
            // 
            // group_edit
            // 
            this.group_edit.Controls.Add(this.buttonEditReg);
            this.group_edit.Controls.Add(this.buttonFinishEdit);
            this.group_edit.Controls.Add(this.button_CLREG);
            this.group_edit.Controls.Add(this.button_CLRAM);
            this.group_edit.Controls.Add(this.button_EditRAM);
            this.group_edit.Location = new System.Drawing.Point(394, 245);
            this.group_edit.Name = "group_edit";
            this.group_edit.Size = new System.Drawing.Size(551, 172);
            this.group_edit.TabIndex = 8;
            this.group_edit.TabStop = false;
            this.group_edit.Text = "编辑";
            // 
            // buttonEditReg
            // 
            this.buttonEditReg.Location = new System.Drawing.Point(311, 30);
            this.buttonEditReg.Name = "buttonEditReg";
            this.buttonEditReg.Size = new System.Drawing.Size(161, 50);
            this.buttonEditReg.TabIndex = 1;
            this.buttonEditReg.Text = "编辑寄存器值";
            this.buttonEditReg.UseVisualStyleBackColor = true;
            this.buttonEditReg.Click += new System.EventHandler(this.buttonEditReg_Click);
            // 
            // buttonFinishEdit
            // 
            this.buttonFinishEdit.Location = new System.Drawing.Point(311, 30);
            this.buttonFinishEdit.Name = "buttonFinishEdit";
            this.buttonFinishEdit.Size = new System.Drawing.Size(161, 50);
            this.buttonFinishEdit.TabIndex = 4;
            this.buttonFinishEdit.Text = "保存寄存器值";
            this.buttonFinishEdit.UseVisualStyleBackColor = true;
            this.buttonFinishEdit.Visible = false;
            this.buttonFinishEdit.Click += new System.EventHandler(this.buttonFinishEdit_Click);
            // 
            // button_CLREG
            // 
            this.button_CLREG.Location = new System.Drawing.Point(311, 101);
            this.button_CLREG.Name = "button_CLREG";
            this.button_CLREG.Size = new System.Drawing.Size(161, 50);
            this.button_CLREG.TabIndex = 3;
            this.button_CLREG.Text = "清空寄存器";
            this.button_CLREG.UseVisualStyleBackColor = true;
            this.button_CLREG.Click += new System.EventHandler(this.button_CLREG_Click);
            // 
            // button_CLRAM
            // 
            this.button_CLRAM.Location = new System.Drawing.Point(79, 101);
            this.button_CLRAM.Name = "button_CLRAM";
            this.button_CLRAM.Size = new System.Drawing.Size(161, 50);
            this.button_CLRAM.TabIndex = 2;
            this.button_CLRAM.Text = "清空内存";
            this.button_CLRAM.UseVisualStyleBackColor = true;
            this.button_CLRAM.Click += new System.EventHandler(this.button_CLRAM_Click);
            // 
            // button_EditRAM
            // 
            this.button_EditRAM.Location = new System.Drawing.Point(79, 30);
            this.button_EditRAM.Name = "button_EditRAM";
            this.button_EditRAM.Size = new System.Drawing.Size(161, 50);
            this.button_EditRAM.TabIndex = 0;
            this.button_EditRAM.Text = "编辑内存单元";
            this.button_EditRAM.UseVisualStyleBackColor = true;
            this.button_EditRAM.Click += new System.EventHandler(this.button_EditRAM_Click);
            // 
            // timer1
            // 
            this.timer1.Enabled = true;
            this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(10F, 25F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.ClientSize = new System.Drawing.Size(959, 594);
            this.Controls.Add(this.group_edit);
            this.Controls.Add(this.group_run);
            this.Controls.Add(this.button_open);
            this.Controls.Add(this.group_CPU);
            this.Controls.Add(this.button_about);
            this.Controls.Add(this.button_exit);
            this.Controls.Add(this.group_RAM);
            this.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.MaximizeBox = false;
            this.Name = "MainForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "CPU Emulator";
            this.Load += new System.EventHandler(this.MainForm_Load);
            this.group_RAM.ResumeLayout(false);
            this.group_CPU.ResumeLayout(false);
            this.group_CPU.PerformLayout();
            this.group_run.ResumeLayout(false);
            this.group_edit.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox group_RAM;
        private System.Windows.Forms.Button button_exit;
        private System.Windows.Forms.Button button_about;
        private System.Windows.Forms.GroupBox group_CPU;
        private System.Windows.Forms.Button button_step;
        private System.Windows.Forms.Button button_auto;
        private System.Windows.Forms.Button button_open;
        private System.Windows.Forms.GroupBox group_run;
        private System.Windows.Forms.OpenFileDialog openFileDialog;
        private System.Windows.Forms.ListView listView1;
        private System.Windows.Forms.ColumnHeader address;
        private System.Windows.Forms.ColumnHeader symbol;
        private System.Windows.Forms.ColumnHeader content;
        private System.Windows.Forms.TextBox textBox_R3;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox textBox_DR;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.TextBox textBox_R2;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.TextBox textBox_AR;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.TextBox textBox_R1;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox textBox_IR;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox textBox_R0;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox textBox_PC;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.GroupBox group_edit;
        private System.Windows.Forms.Button buttonEditReg;
        private System.Windows.Forms.Button button_EditRAM;
        private System.Windows.Forms.Button button_CLREG;
        private System.Windows.Forms.Button button_CLRAM;
        private System.Windows.Forms.Button buttonFinishEdit;
        private System.Windows.Forms.Button button_pause;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.TextBox textBox_INT_addr;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.Timer timer1;
    }
}

