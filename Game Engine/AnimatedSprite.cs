using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace CPI311.GameEngine
{
    public class AnimatedSprite : Sprite
    {
        public int Frames { get; set; }
        public float Frame { get; set; }
        public float Speed { get; set; }
        public int x { get; set; }
        public int y { get; set; }

        public AnimatedSprite(Texture2D texture, int frames = 1) : base(texture)
        {
            int x = 0;
            int y = 0;
            Frames = frames;
            Frame = 0;
            Speed = 1;
            Source = new Rectangle(x, y, Texture.Width/8, Texture.Height/5);
        }

        public override void Update()
        {
            Frame += Speed * Time.ElapsedGameTime;
            //logic to restart animation from beginning
            if(x == Texture.Width - Texture.Width / 8)
            {
                x = 0;
            }
            
        }
    }
}
