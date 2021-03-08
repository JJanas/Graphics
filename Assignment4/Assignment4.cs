using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using CPI411.SimpleEngine;

namespace Assignment4
{
    /// <summary>
    /// This is the main type for your game.
    /// </summary>
    public class Assignment4 : Game
    {
        GraphicsDeviceManager graphics;
        SpriteBatch spriteBatch;

        ParticleManager particleManager;
        System.Random random;

        Effect effect;
        Model model;
        Texture2D[] textures = new Texture2D[3];
        string[] shape = new string[3];
        string[] technique = new string[2];
        SpriteFont font;
        Matrix world = Matrix.Identity;
        Matrix view = Matrix.CreateLookAt(new Vector3(0, 0, 5), Vector3.Zero, Vector3.Up);
        Matrix projection = Matrix.CreatePerspectiveFieldOfView(MathHelper.ToRadians(90), 800f / 600f, 0.01f, 1000f);
        Vector3 cameraPosition = new Vector3(0, 0, 10);

        float distance = 10;
        Vector3 cameraTarget = new Vector3(0, 0, 0);

        Vector4 Ambient = new Vector4(1, 1, 1, 1);
        float AmbientIntensity = 0.7f;
        Vector4 DiffuseColor = new Vector4(.7f, .7f, .7f, 1);
        float DiffuseIntensity = 1.0f;
        Vector4 specularColor = new Vector4(1, 1, 1, 1);
        float specularIntensity = 1.0f;
        float shininess = 10.0f;
        Vector3 lightPosition = new Vector3(1, 1, 1);
        Matrix lightView;
        Matrix lightProjection;

        float angle, angle2, angleL, angleL2 = 0.5f;
        Vector3 cameraOffset = new Vector3(0, 0, 0);
        float cameraX = 0;
        float cameraY = 0;
        float cameraZ = 20;
        float lightX = 1;
        float lightY = 1;
        float lightZ = 0;
        float angle3 = 0;
        float angle4 = 0;
        float offset = 0;
        float offset2 = 0;
        int Technique = 1;
        int pattern = 0;
        int number = 0;
        int s = -5;
        int velocity = 1;
        int wind = 0;
        bool swap = false;
        bool draw = false;
        bool help = false;
        MouseState previousMouseState;

        Vector3 particlePosition;

        public Assignment4()
        {
            graphics = new GraphicsDeviceManager(this);
            Content.RootDirectory = "Content";
            graphics.GraphicsProfile = GraphicsProfile.HiDef;
            graphics.PreferredBackBufferWidth = 1280;
            graphics.PreferredBackBufferHeight = 720;
            graphics.ApplyChanges();
        }

        /// <summary>
        /// Allows the game to perform any initialization it needs to before starting to run.
        /// This is where it can query for any required services and load any non-graphic
        /// related content.  Calling base.Initialize will enumerate through any components
        /// and initialize them as well.
        /// </summary>
        protected override void Initialize()
        {
            // TODO: Add your initialization logic here

            base.Initialize();
        }

        /// <summary>
        /// LoadContent will be called once per game and is the place to load
        /// all of your content.
        /// </summary>
        protected override void LoadContent()
        {
            // Create a new SpriteBatch, which can be used to draw textures.
            spriteBatch = new SpriteBatch(GraphicsDevice);

            model = Content.Load<Model>("Plane");
            effect = Content.Load<Effect>("ParticleShader");
            font = Content.Load<SpriteFont>("Font");
            textures[0] = Content.Load<Texture2D>("fire");
            textures[1] = Content.Load<Texture2D>("smoke");
            textures[2] = Content.Load<Texture2D>("water");
            shape[0] = "Square";
            shape[1] = "Curve";
            shape[2] = "Ring";
            technique[0] = "Phong";
            technique[1] = "Textured";
            random = new System.Random();
            particleManager = new ParticleManager(GraphicsDevice, 100);
            particlePosition = new Vector3(0, 0, 0);
        }

        /// <summary>
        /// UnloadContent will be called once per game and is the place to unload
        /// game-specific content.
        /// </summary>
        protected override void UnloadContent()
        {
            // TODO: Unload any non ContentManager content here
        }

        /// <summary>
        /// Allows the game to run logic such as updating the world,
        /// checking for collisions, gathering input, and playing audio.
        /// </summary>
        /// <param name="gameTime">Provides a snapshot of timing values.</param>
        protected override void Update(GameTime gameTime)
        {
            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed || Keyboard.GetState().IsKeyDown(Keys.Escape))
                Exit();

            if (pattern == 0 && Keyboard.GetState().IsKeyDown(Keys.P))
            {
                Particle particle = particleManager.getNext();
                particle.Position = particlePosition;
                particle.Velocity = new Vector3(s, random.Next(1, 5), 0);
                particle.Acceleration = new Vector3(0, -10, 0);
                particle.MaxAge = 1;
                particle.Init();
                if (s == 5)
                {
                    swap = true;
                }
                if (s == -5)
                {
                    swap = false;
                }
                if(swap == false)
                {
                    s++;
                }
                if (swap == true)
                {
                    s--;
                }
                
            }
            if (pattern == 1 && Keyboard.GetState().IsKeyDown(Keys.P))
            {
                Particle particle = particleManager.getNext();
                particle.Position = particlePosition;
                particle.Velocity = new Vector3(random.Next(-5, 5), 5, 0);
                particle.Acceleration = new Vector3(0, 0, 0);
                particle.MaxAge = 1;
                particle.Init();
            }
            if (pattern == 2 && Keyboard.GetState().IsKeyDown(Keys.P))
            {
                for (int i = 0; i < 60; i++)
                {
                    double angle = System.Math.PI * (i * 6) / 180.0;
                    Particle particle = particleManager.getNext();
                    particle.Position = particlePosition;
                    particle.Velocity = new Vector3(10f * (float)System.Math.Sin(angle),
                        0,
                        10f * (float)System.Math.Cos(angle));
                    particle.Acceleration = new Vector3(0, 0, 0);
                    particle.MaxAge = 1;
                    particle.Init();
                }
            }
            particleManager.Update(gameTime.ElapsedGameTime.Milliseconds * 0.001f);
            /*Camera Controls*/
            if (Mouse.GetState().LeftButton == ButtonState.Pressed)
            {
                offset = (Mouse.GetState().X - previousMouseState.X) * .1f;
                offset2 = (Mouse.GetState().Y - previousMouseState.Y) * .1f;
                angle += (offset);
                angle2 += (offset2);
            }
            if (Mouse.GetState().RightButton == ButtonState.Pressed)
            {
                offset = (Mouse.GetState().Y - previousMouseState.Y) * .1f;
                cameraZ += offset;
            }
            if (Mouse.GetState().MiddleButton == ButtonState.Pressed)
            {
                offset = (Mouse.GetState().X - previousMouseState.X) * .1f;
                offset2 = (Mouse.GetState().Y - previousMouseState.Y) * .1f;
                cameraX += offset;
                cameraY += offset2;
            }
            /*Light controls*/
            if (Keyboard.GetState().IsKeyDown(Keys.Up))
            {
                angle4 += 0.02f;
            }
            if (Keyboard.GetState().IsKeyDown(Keys.Down))
            {
                angle4 -= 0.02f;
            }
            if (Keyboard.GetState().IsKeyDown(Keys.Left))
            {
                angle3 -= 0.02f;
            }
            if (Keyboard.GetState().IsKeyDown(Keys.Right))
            {
                angle3 += 0.02f;
            }
            /*Light and Camera reset*/
            if (Keyboard.GetState().IsKeyDown(Keys.S))
            {
                cameraX = 0;
                cameraY = 0;
                cameraZ = 20;
                angle = 0;
                angle2 = 0;
                angle3 = 0;
                angle4 = 0;
                lightX = 1;
                lightY = 1;
                lightZ = 0;
            }

            /*Model Swaps*/
            if (Keyboard.GetState().IsKeyDown(Keys.D1))
            {
                Technique = 0;
            }
            if (Keyboard.GetState().IsKeyDown(Keys.D2))
            {
                Technique = 1;
                number = 0;
            }
            if (Keyboard.GetState().IsKeyDown(Keys.D3))
            {
                Technique = 1;
                number = 1;
            }
            if (Keyboard.GetState().IsKeyDown(Keys.D4))
            {
                Technique = 1;
                number = 2;
            }

            /*Shader controls*/
            if (Keyboard.GetState().IsKeyDown(Keys.F1))
            {
                Particle particle = particleManager.getNext();
                particle.Position = particlePosition;
                particle.Velocity = new Vector3(0, 5, 0);
                particle.Acceleration = new Vector3(0, 0, 0);
                particle.MaxAge = 1;
                particle.Init();
            }
            if (Keyboard.GetState().IsKeyDown(Keys.F2))
            {
                Particle particle = particleManager.getNext();
                particle.Position = particlePosition;
                particle.Velocity = new Vector3(random.Next(1, 5), random.Next(1, 5), 0);
                particle.Acceleration = new Vector3(random.Next(0, 10), -10, 0);
                particle.MaxAge = 1;
                particle.Init();
            }
            if (Keyboard.GetState().IsKeyDown(Keys.F3))
            {
                Particle particle = particleManager.getNext();
                particle.Position = particlePosition;
                particle.Velocity = new Vector3(random.Next((velocity + wind), (velocity + wind + 5)), random.Next(velocity, (velocity + 5)), 0);
                particle.Acceleration = new Vector3(random.Next(0, 10), -10, 0);
                particle.MaxAge = 1;
                particle.Init();
            }
            if (Keyboard.GetState().IsKeyDown(Keys.F4))
            {
                if (pattern < 2)
                {
                    pattern++;
                    return;
                }
                if (pattern == 2)
                {
                    pattern = 0;
                    return;
                }
            }

            if (Keyboard.GetState().IsKeyDown(Keys.V) && (!Keyboard.GetState().IsKeyDown(Keys.LeftShift) && !Keyboard.GetState().IsKeyDown(Keys.RightShift)))
            {
                velocity++;
            }
            if (Keyboard.GetState().IsKeyDown(Keys.W) && (!Keyboard.GetState().IsKeyDown(Keys.LeftShift) && !Keyboard.GetState().IsKeyDown(Keys.RightShift)))
            {
                wind++;
            }
            if (Keyboard.GetState().IsKeyDown(Keys.V) && (Keyboard.GetState().IsKeyDown(Keys.LeftShift) || Keyboard.GetState().IsKeyDown(Keys.RightShift)))
            {
                velocity--;
            }
            if (Keyboard.GetState().IsKeyDown(Keys.W) && (Keyboard.GetState().IsKeyDown(Keys.LeftShift) || Keyboard.GetState().IsKeyDown(Keys.RightShift)))
            {
                wind--;
            }
            /*Info*/
            if (Keyboard.GetState().IsKeyDown(Keys.H))
            {
                if (draw == false)
                {
                    draw = true;
                    return;
                }
                if (draw == true)
                {
                    draw = false;
                    return;
                }
            }
            /*Help*/
            if (Keyboard.GetState().IsKeyDown(Keys.OemQuestion))
            {
                if (help == false)
                {
                    help = true;
                    return;
                }
                if (help == true)
                {
                    help = false;
                    return;
                }
            }


            /*Camera*/
            Vector3 camera = Vector3.Transform(
                new Vector3(cameraX, cameraY, cameraZ),
                Matrix.CreateRotationX(angle2) * Matrix.CreateRotationY(angle)
                );
            /*Light*/
            Vector3 light = Vector3.Transform(
                new Vector3(lightX, lightY, lightZ),
                Matrix.CreateRotationX(angle4) * Matrix.CreateRotationY(angle3)
                );

            lightPosition += light;

            world = Matrix.Identity;
            view = Matrix.CreateLookAt(
                camera,
                Vector3.Zero,
                Vector3.UnitY
                );
            projection = Matrix.CreatePerspectiveFieldOfView(
                MathHelper.ToRadians(90),
                GraphicsDevice.Viewport.AspectRatio,
                0.1f, 100);

            previousMouseState = Mouse.GetState();
            offset = 0;
            offset2 = 0;

            base.Update(gameTime);
        }

        /// <summary>
        /// This is called when the game should draw itself.
        /// </summary>
        /// <param name="gameTime">Provides a snapshot of timing values.</param>
        protected override void Draw(GameTime gameTime)
        {
            GraphicsDevice.Clear(Color.CornflowerBlue);

            GraphicsDevice.BlendState = BlendState.AlphaBlend;
            GraphicsDevice.DepthStencilState = new DepthStencilState();

            effect.CurrentTechnique = effect.Techniques[Technique];
            effect.CurrentTechnique.Passes[0].Apply();
            effect.Parameters["InverseCamera"].SetValue(view);
            effect.Parameters["World"].SetValue(Matrix.Identity);
            effect.Parameters["View"].SetValue(view);
            effect.Parameters["Projection"].SetValue(projection);
            effect.Parameters["Texture"].SetValue(textures[number]);
            effect.Parameters["AmbientColor"].SetValue(Ambient);
            effect.Parameters["AmbientIntensity"].SetValue(AmbientIntensity);
            effect.Parameters["DiffuseColor"].SetValue(DiffuseColor);
            effect.Parameters["DiffuseIntensity"].SetValue(DiffuseIntensity);
            effect.Parameters["SpecularColor"].SetValue(specularColor);
            effect.Parameters["SpecularIntensity"].SetValue(specularIntensity);
            effect.Parameters["Shininess"].SetValue(shininess);
            effect.Parameters["CameraPosition"].SetValue(cameraPosition);
            effect.Parameters["LightPosition"].SetValue(lightPosition);
            /*
            effect.CurrentTechnique = effect.Techniques[0];
            foreach (EffectPass pass in effect.CurrentTechnique.Passes)
            {
                foreach (ModelMesh mesh in model.Meshes)
                {
                    foreach (ModelMeshPart part in mesh.MeshParts)
                    {
                        effect.Parameters["World"].SetValue(mesh.ParentBone.Transform);
                        effect.Parameters["View"].SetValue(view);
                        effect.Parameters["Projection"].SetValue(projection);

                        //Matrix worldInverseTranspose =
                           //Matrix.Transpose(Matrix.Invert(mesh.ParentBone.Transform));
                        //effect.Parameters["WorldInverseTranspose"].SetValue(worldInverseTranspose);

                        pass.Apply();
                        GraphicsDevice.SetVertexBuffer(part.VertexBuffer);
                        GraphicsDevice.Indices = part.IndexBuffer;

                        GraphicsDevice.DrawIndexedPrimitives(
                                PrimitiveType.TriangleList, part.VertexOffset, 0,
                                part.NumVertices, part.StartIndex, part.PrimitiveCount);
                    }
                }

            }
            */

            particleManager.Draw(GraphicsDevice);
            model.Draw(world, view, projection);
            GraphicsDevice.RasterizerState = RasterizerState.CullNone;
            if (draw)
            {
                spriteBatch.Begin();
                spriteBatch.DrawString(font, "CameraAngle:" + angle, new Vector2(0, 0), Color.White);
                spriteBatch.DrawString(font, "CameraAngle2:" + angle2, new Vector2(0, 15), Color.White);
                spriteBatch.DrawString(font, "LightAngle:" + angle3, new Vector2(0, 30), Color.White);
                spriteBatch.DrawString(font, "LightAngle2:" + angle4, new Vector2(0, 45), Color.White);
                spriteBatch.DrawString(font, "Texture:" + textures[number], new Vector2(0, 60), Color.White);
                spriteBatch.DrawString(font, "Shape:" + shape[pattern], new Vector2(0, 75), Color.White);
                spriteBatch.DrawString(font, "Technique:" + technique[Technique], new Vector2(0, 90), Color.White);
                spriteBatch.DrawString(font, "Velocity Mod:" + velocity, new Vector2(0, 105), Color.White);
                spriteBatch.DrawString(font, "Wind:" + wind, new Vector2(0, 120), Color.White);
                spriteBatch.End();
            }
            if (help)
            {
                spriteBatch.Begin();
                spriteBatch.DrawString(font, "Help: ?", new Vector2(850, 0), Color.White);
                spriteBatch.DrawString(font, "Camera Rotation: Hold left click and move mouse", new Vector2(850, 15), Color.White);
                spriteBatch.DrawString(font, "Camera Zoom: Hold right click and move mouse vertically", new Vector2(850, 30), Color.White);
                spriteBatch.DrawString(font, "Camera Translation: Hold middle click and move mouse", new Vector2(850, 45), Color.White);
                spriteBatch.DrawString(font, "Light Rotation: Use arrow keys", new Vector2(850, 60), Color.White);
                spriteBatch.DrawString(font, "Debug: H", new Vector2(850, 75), Color.White);
                spriteBatch.DrawString(font, "Press F1 for basic fountain", new Vector2(850, 90), Color.White);
                spriteBatch.DrawString(font, "Press F2 for medium fountain", new Vector2(850, 105), Color.White);
                spriteBatch.DrawString(font, "Press F3 for advanced fountain", new Vector2(850, 120), Color.White);
                spriteBatch.DrawString(font, "Change shape: F4", new Vector2(850, 135), Color.White);
                spriteBatch.DrawString(font, "Particle texture 1-4", new Vector2(850, 150), Color.White);
                spriteBatch.DrawString(font, "Emit particles in current shape: P", new Vector2(850, 165), Color.White);
                spriteBatch.DrawString(font, "Change shape: F4", new Vector2(850, 180), Color.White);
                spriteBatch.DrawString(font, "v/V to increase/decrease velocity", new Vector2(850, 195), Color.White);
                spriteBatch.DrawString(font, "w/W to increase/decrease wind", new Vector2(850, 210), Color.White);
                spriteBatch.End();
            }
            base.Draw(gameTime);
        }
    }
}
