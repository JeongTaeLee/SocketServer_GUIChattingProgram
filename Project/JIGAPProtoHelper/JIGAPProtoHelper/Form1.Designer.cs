namespace JIGAPProtoHelper
{
    partial class Form1
    {
        /// <summary>
        /// 필수 디자이너 변수입니다.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 사용 중인 모든 리소스를 정리합니다.
        /// </summary>
        /// <param name="disposing">관리되는 리소스를 삭제해야 하면 true이고, 그렇지 않으면 false입니다.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form 디자이너에서 생성한 코드

        /// <summary>
        /// 디자이너 지원에 필요한 메서드입니다. 
        /// 이 메서드의 내용을 코드 편집기로 수정하지 마세요.
        /// </summary>
        private void InitializeComponent()
        {
            this.label1 = new System.Windows.Forms.Label();
            this.protoAppTextBox = new System.Windows.Forms.TextBox();
            this.protoAppPathOpenBtn = new System.Windows.Forms.Button();
            this.outputPathTextBox = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.outputPathOpenBtn = new System.Windows.Forms.Button();
            this.outputButton = new System.Windows.Forms.Button();
            this.outputPathDialog = new System.Windows.Forms.FolderBrowserDialog();
            this.protoFilePathTextBox = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.protoFilePathOpenBtn = new System.Windows.Forms.Button();
            this.protoFileOpenDialog = new System.Windows.Forms.OpenFileDialog();
            this.protoAppOpenDialog = new System.Windows.Forms.OpenFileDialog();
            this.checkBox1 = new System.Windows.Forms.CheckBox();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.Font = new System.Drawing.Font("맑은 고딕", 24F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label1.Location = new System.Drawing.Point(66, 9);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(286, 43);
            this.label1.TabIndex = 0;
            this.label1.Text = "JIGAPProtoHelper";
            this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // protoAppTextBox
            // 
            this.protoAppTextBox.Location = new System.Drawing.Point(12, 82);
            this.protoAppTextBox.Name = "protoAppTextBox";
            this.protoAppTextBox.ReadOnly = true;
            this.protoAppTextBox.Size = new System.Drawing.Size(302, 21);
            this.protoAppTextBox.TabIndex = 1;
            // 
            // protoAppPathOpenBtn
            // 
            this.protoAppPathOpenBtn.Location = new System.Drawing.Point(324, 80);
            this.protoAppPathOpenBtn.Name = "protoAppPathOpenBtn";
            this.protoAppPathOpenBtn.Size = new System.Drawing.Size(70, 22);
            this.protoAppPathOpenBtn.TabIndex = 2;
            this.protoAppPathOpenBtn.Text = "열기";
            this.protoAppPathOpenBtn.UseVisualStyleBackColor = true;
            this.protoAppPathOpenBtn.Click += new System.EventHandler(this.ProtoAppPathButton_Click);
            // 
            // outputPathTextBox
            // 
            this.outputPathTextBox.Location = new System.Drawing.Point(12, 179);
            this.outputPathTextBox.Name = "outputPathTextBox";
            this.outputPathTextBox.ReadOnly = true;
            this.outputPathTextBox.Size = new System.Drawing.Size(302, 21);
            this.outputPathTextBox.TabIndex = 3;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(12, 67);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(117, 12);
            this.label2.TabIndex = 4;
            this.label2.Text = "프로토버퍼 컴파일러";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(12, 164);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(57, 12);
            this.label3.TabIndex = 5;
            this.label3.Text = "출력 위치";
            // 
            // outputPathOpenBtn
            // 
            this.outputPathOpenBtn.Location = new System.Drawing.Point(324, 179);
            this.outputPathOpenBtn.Name = "outputPathOpenBtn";
            this.outputPathOpenBtn.Size = new System.Drawing.Size(70, 21);
            this.outputPathOpenBtn.TabIndex = 6;
            this.outputPathOpenBtn.Text = "열기";
            this.outputPathOpenBtn.UseVisualStyleBackColor = true;
            this.outputPathOpenBtn.Click += new System.EventHandler(this.OutputPathOpen_Click);
            // 
            // outputButton
            // 
            this.outputButton.Location = new System.Drawing.Point(14, 245);
            this.outputButton.Name = "outputButton";
            this.outputButton.Size = new System.Drawing.Size(379, 44);
            this.outputButton.TabIndex = 7;
            this.outputButton.Text = "출력";
            this.outputButton.UseVisualStyleBackColor = true;
            this.outputButton.Click += new System.EventHandler(this.OutputButton_Click);
            // 
            // protoFilePathTextBox
            // 
            this.protoFilePathTextBox.Location = new System.Drawing.Point(12, 129);
            this.protoFilePathTextBox.Name = "protoFilePathTextBox";
            this.protoFilePathTextBox.ReadOnly = true;
            this.protoFilePathTextBox.Size = new System.Drawing.Size(301, 21);
            this.protoFilePathTextBox.TabIndex = 8;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(10, 114);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(93, 12);
            this.label4.TabIndex = 9;
            this.label4.Text = ".proto 파일 위치";
            // 
            // protoFilePathOpenBtn
            // 
            this.protoFilePathOpenBtn.Location = new System.Drawing.Point(324, 129);
            this.protoFilePathOpenBtn.Name = "protoFilePathOpenBtn";
            this.protoFilePathOpenBtn.Size = new System.Drawing.Size(70, 21);
            this.protoFilePathOpenBtn.TabIndex = 10;
            this.protoFilePathOpenBtn.Text = "열기";
            this.protoFilePathOpenBtn.UseVisualStyleBackColor = true;
            this.protoFilePathOpenBtn.Click += new System.EventHandler(this.ProtoFilePathOpenButton_Click);
            // 
            // protoFileOpenDialog
            // 
            this.protoFileOpenDialog.FileName = "openFileDialog1";
            this.protoFileOpenDialog.Filter = "proto|*.proto";
            // 
            // protoAppOpenDialog
            // 
            this.protoAppOpenDialog.FileName = "openFileDialog1";
            this.protoAppOpenDialog.Filter = "실행파일|*.exe";
            // 
            // checkBox1
            // 
            this.checkBox1.AutoSize = true;
            this.checkBox1.BackColor = System.Drawing.SystemColors.Control;
            this.checkBox1.Location = new System.Drawing.Point(22, 206);
            this.checkBox1.Name = "checkBox1";
            this.checkBox1.Size = new System.Drawing.Size(112, 16);
            this.checkBox1.TabIndex = 11;
            this.checkBox1.Text = "파일위치랑 동일";
            this.checkBox1.UseVisualStyleBackColor = false;
            this.checkBox1.CheckedChanged += new System.EventHandler(this.CheckBox1_CheckedChanged_1);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(411, 301);
            this.Controls.Add(this.checkBox1);
            this.Controls.Add(this.protoFilePathOpenBtn);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.protoFilePathTextBox);
            this.Controls.Add(this.outputButton);
            this.Controls.Add(this.outputPathOpenBtn);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.outputPathTextBox);
            this.Controls.Add(this.protoAppPathOpenBtn);
            this.Controls.Add(this.protoAppTextBox);
            this.Controls.Add(this.label1);
            this.Name = "Form1";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox protoAppTextBox;
        private System.Windows.Forms.Button protoAppPathOpenBtn;
        private System.Windows.Forms.TextBox outputPathTextBox;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Button outputPathOpenBtn;
        private System.Windows.Forms.Button outputButton;
        private System.Windows.Forms.FolderBrowserDialog outputPathDialog;
        private System.Windows.Forms.TextBox protoFilePathTextBox;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Button protoFilePathOpenBtn;
        private System.Windows.Forms.OpenFileDialog protoFileOpenDialog;
        private System.Windows.Forms.OpenFileDialog protoAppOpenDialog;
        private System.Windows.Forms.CheckBox checkBox1;
    }
}

