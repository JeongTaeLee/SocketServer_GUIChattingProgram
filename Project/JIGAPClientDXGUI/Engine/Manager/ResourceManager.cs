using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SharpDX;
using SharpDX.Direct3D9;

namespace JIGAPClientDXGUI
{
    partial class texture : IDisposable
    {
        public Texture d3dTex { get; private set; } = null;
        public ImageInformation d3dInfo { get; private set; } = new ImageInformation();

        public texture(Texture inTex, ImageInformation inInfo)
        {
            d3dTex = inTex;
            d3dInfo = inInfo;
        }

        public void Draw()
        {
            if (d3dTex != null)
                DXManager.Instance.d3dSprite.Draw(d3dTex, Color.White);

        }

        public void Draw(Color color)
        {
            if (d3dTex != null)
                DXManager.Instance.d3dSprite.Draw(d3dTex, color);

        }



        public void Dispose()
        {
            d3dTex.Dispose();
        }
    }

    partial class ResourceManager : IDisposable
    {
        private static ResourceManager instance = null;
        public static ResourceManager Instance
        {
            get
            {
                if (instance == null)
                    instance = new ResourceManager();

                return instance;
            }
        }


        private Dictionary<string, texture> _textures = new Dictionary<string, texture>();
        private Dictionary<string, Font> _fonts = new Dictionary<string, Font>();
    }


    partial class ResourceManager : IDisposable
    {

        public texture LoadTexture(string key, string path = "None")
        {
            texture myTex = null;
          
            if (!_textures.TryGetValue(key, out myTex))
            { 
                ImageInformation info;

                Texture d3dTex = Texture.FromFile(DXManager.Instance.d3dDevice, path,
                D3DX.DefaultNonPowerOf2, D3DX.DefaultNonPowerOf2, 1, Usage.None, Format.Unknown, Pool.Managed, Filter.None, Filter.None, 0, out info);

                myTex = new texture(d3dTex, info);

                _textures.Add(key, myTex);
            }

            return myTex;
        }
        public Font LoadFont(string key, int inFontSize = 0, FontWeight inFontWeight = FontWeight.Bold)
        {
            Font font = null;

            if (!_fonts.TryGetValue(key, out font))
            {
                font = new Font(DXManager.Instance.d3dDevice, inFontSize,
                0,
                inFontWeight,
                0,
                false,
                FontCharacterSet.Hangul,
                FontPrecision.Default,
                FontQuality.Default,
                FontPitchAndFamily.Default,
                "맑은 고딕");

                _fonts.Add(key, font);
            }

            return font;
        }


        public void Dispose()
        {
            foreach(KeyValuePair<string, texture> tex in _textures)
                tex.Value.Dispose();

            foreach (KeyValuePair<string, Font> font in _fonts)
                font.Value.Dispose();



            _textures.Clear();
            _fonts.Clear();
            GC.SuppressFinalize(this);
        }
    }

}
