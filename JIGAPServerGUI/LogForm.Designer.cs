namespace JIGAPServerGUI
{
    partial class LogForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.LogTextBox = new System.Windows.Forms.TextBox();
            this.ServerCloseButton = new System.Windows.Forms.Button();
            this.LogFormCloseButton = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // LogTextBox
            // 
            this.LogTextBox.Location = new System.Drawing.Point(14, 12);
            this.LogTextBox.Multiline = true;
            this.LogTextBox.Name = "LogTextBox";
            this.LogTextBox.ReadOnly = true;
            this.LogTextBox.Size = new System.Drawing.Size(329, 382);
            this.LogTextBox.TabIndex = 0;
            // 
            // ServerCloseButton
            // 
            this.ServerCloseButton.Location = new System.Drawing.Point(14, 410);
            this.ServerCloseButton.Name = "ServerCloseButton";
            this.ServerCloseButton.Size = new System.Drawing.Size(159, 28);
            this.ServerCloseButton.TabIndex = 1;
            this.ServerCloseButton.Text = "서버 종료";
            this.ServerCloseButton.UseVisualStyleBackColor = true;
            this.ServerCloseButton.Click += new System.EventHandler(this.ServerCloseButton_Click);
            // 
            // LogFormCloseButton
            // 
            this.LogFormCloseButton.Location = new System.Drawing.Point(180, 410);
            this.LogFormCloseButton.Name = "LogFormCloseButton";
            this.LogFormCloseButton.Size = new System.Drawing.Size(163, 28);
            this.LogFormCloseButton.TabIndex = 2;
            this.LogFormCloseButton.Text = "로그 창 닫기";
            this.LogFormCloseButton.UseVisualStyleBackColor = true;
            this.LogFormCloseButton.Click += new System.EventHandler(this.LogFormCloseButton_Click);
            // 
            // LogForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(355, 450);
            this.ControlBox = false;
            this.Controls.Add(this.LogFormCloseButton);
            this.Controls.Add(this.ServerCloseButton);
            this.Controls.Add(this.LogTextBox);
            this.Name = "LogForm";
            this.Text = "LogForm";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox LogTextBox;
        private System.Windows.Forms.Button ServerCloseButton;
        private System.Windows.Forms.Button LogFormCloseButton;
    }
}