using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;

namespace Final
{
    /// <summary>
    /// This is the main type for your game.
    /// </summary>
    public class LightScattering : Game
    {
        GraphicsDeviceManager graphics;
        SpriteBatch spriteBatch;

        SpriteFont font;
        Effect effect;
        Model model;
        Matrix world = Matrix.Identity;
        Matrix view = Matrix.CreateLookAt(new Vector3(0, 0, 0), new Vector3(0, 0, 0), Vector3.UnitY);
        Matrix projection = Matrix.CreatePerspectiveFieldOfView(MathHelper.ToRadians(45), 800f / 600f, 0.1f, 100f);
        Matrix lightView = Matrix.CreateLookAt(new Vector3(0, 0, 10), -Vector3.UnitZ, Vector3.UnitY);
        Matrix lightProjection = Matrix.CreatePerspectiveFieldOfView(MathHelper.PiOver2, 1f, 1f, 100f);
        Vector3 cameraPosition, cameraTarget, lightPosition;

        Vector4 Ambient = new Vector4(0.1f, 0.1f, 0.1f, 1.0f);
        float AmbientIntensity = 1.0f;
        Vector4 DiffuseColor = new Vector4(1.0f, 1.0f, 1.0f, 1.0f);
        float DiffuseIntensity = 1.0f;
        Vector4 specularColor = new Vector4(1.0f, 1.0f, 1.0f, 1.0f);
        float specularIntensity = 1.0f;
        float shininess = 10.0f;
        float Exposure = 0.1f;
        float Weight = 0.59f;
        float Decay = 0.98f;
        float Density = 0.93f;

        bool draw = false;
        bool help = false;
        Model[] models = new Model[3];
        string[] modelName = new string[3];
        int number = 0;

        RenderTarget2D renderTarget, preRenderTarget;
        Texture2D prePass;
        Texture2D litScene;

        float angle, angle2, angleL, angleL2;
        float distance = 20;
        MouseState preMouse;
        

        public LightScattering()
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
            modelName[0] = "Torus";
            modelName[1] = "Box";
            modelName[2] = "Teapot";
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

            models[0] = Content.Load<Model>("torus");
            models[1] = Content.Load<Model>("Box");
            models[2] = Content.Load<Model>("Teapot");
            model = models[0];
            effect = Content.Load<Effect>("LightScattering");
            font = Content.Load<SpriteFont>("Font");

            preRenderTarget = new RenderTarget2D(GraphicsDevice, Window.ClientBounds.Width,
                Window.ClientBounds.Height, false, SurfaceFormat.Color, DepthFormat.Depth24,
                0, RenderTargetUsage.PlatformContents);
            renderTarget = new RenderTarget2D(GraphicsDevice, Window.ClientBounds.Width,
                Window.ClientBounds.Height, false, SurfaceFormat.Color, DepthFormat.Depth24,
                0, RenderTargetUsage.PlatformContents);
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

            if (Keyboard.GetState().IsKeyDown(Keys.Left))
            {
                angleL += 0.02f;
            }
            if (Keyboard.GetState().IsKeyDown(Keys.Right))
            {
                angleL -= 0.02f;
            }
            if (Keyboard.GetState().IsKeyDown(Keys.Up))
            {
                angleL2 += 0.02f;
            }
            if (Keyboard.GetState().IsKeyDown(Keys.Down))
            {
                angleL2 -= 0.02f;
            }
            if (Keyboard.GetState().IsKeyDown(Keys.W) && (!Keyboard.GetState().IsKeyDown(Keys.LeftShift) && !Keyboard.GetState().IsKeyDown(Keys.RightShift)))
            {
                Weight += 0.01f;
            }
            if (Keyboard.GetState().IsKeyDown(Keys.W) && (Keyboard.GetState().IsKeyDown(Keys.LeftShift) || Keyboard.GetState().IsKeyDown(Keys.RightShift)))
            {
                Weight -= 0.01f;
            }
            if (Keyboard.GetState().IsKeyDown(Keys.D) && (!Keyboard.GetState().IsKeyDown(Keys.LeftShift) && !Keyboard.GetState().IsKeyDown(Keys.RightShift)))
            {
                Decay += 0.01f;
            }
            if (Keyboard.GetState().IsKeyDown(Keys.D) && (Keyboard.GetState().IsKeyDown(Keys.LeftShift) || Keyboard.GetState().IsKeyDown(Keys.RightShift)))
            {
                Decay -= 0.01f;
            }
            if (Keyboard.GetState().IsKeyDown(Keys.E) && (!Keyboard.GetState().IsKeyDown(Keys.LeftShift) && !Keyboard.GetState().IsKeyDown(Keys.RightShift)))
            {
                Exposure += 0.01f;
            }
            if (Keyboard.GetState().IsKeyDown(Keys.E) && (Keyboard.GetState().IsKeyDown(Keys.LeftShift) || Keyboard.GetState().IsKeyDown(Keys.RightShift)))
            {
                Exposure -= 0.01f;
            }
            if (Keyboard.GetState().IsKeyDown(Keys.T) && (!Keyboard.GetState().IsKeyDown(Keys.LeftShift) && !Keyboard.GetState().IsKeyDown(Keys.RightShift)))
            {
                Density += 0.01f;
            }
            if (Keyboard.GetState().IsKeyDown(Keys.T) && (Keyboard.GetState().IsKeyDown(Keys.LeftShift) || Keyboard.GetState().IsKeyDown(Keys.RightShift)))
            {
                Density -= 0.01f;
            }
            if (Keyboard.GetState().IsKeyDown(Keys.R) && (!Keyboard.GetState().IsKeyDown(Keys.LeftShift) && !Keyboard.GetState().IsKeyDown(Keys.RightShift)))
            {
                Ambient.X += 0.1f;
            }
            if (Keyboard.GetState().IsKeyDown(Keys.R) && (Keyboard.GetState().IsKeyDown(Keys.LeftShift) || Keyboard.GetState().IsKeyDown(Keys.RightShift)))
            {
                Ambient.X -= 0.1f;
            }
            if (Keyboard.GetState().IsKeyDown(Keys.G) && (!Keyboard.GetState().IsKeyDown(Keys.LeftShift) && !Keyboard.GetState().IsKeyDown(Keys.RightShift)))
            {
                Ambient.Y += 0.1f;
            }
            if (Keyboard.GetState().IsKeyDown(Keys.G) && (Keyboard.GetState().IsKeyDown(Keys.LeftShift) || Keyboard.GetState().IsKeyDown(Keys.RightShift)))
            {
                Ambient.Y -= 0.1f;
            }
            if (Keyboard.GetState().IsKeyDown(Keys.B) && (!Keyboard.GetState().IsKeyDown(Keys.LeftShift) && !Keyboard.GetState().IsKeyDown(Keys.RightShift)))
            { 
                Ambient.Z += 0.1f;
            }
            if (Keyboard.GetState().IsKeyDown(Keys.B) && (Keyboard.GetState().IsKeyDown(Keys.LeftShift) || Keyboard.GetState().IsKeyDown(Keys.RightShift)))
            {
                Ambient.Z -= 0.1f;
            }
            if (Keyboard.GetState().IsKeyDown(Keys.S) && (!Keyboard.GetState().IsKeyDown(Keys.LeftShift) && !Keyboard.GetState().IsKeyDown(Keys.RightShift)))
            {
                shininess += 0.1f;
            }
            if (Keyboard.GetState().IsKeyDown(Keys.S) && (Keyboard.GetState().IsKeyDown(Keys.LeftShift) || Keyboard.GetState().IsKeyDown(Keys.RightShift)))
            {
                shininess -= 0.1f;
            }
            /*Model Swaps*/
            if (Keyboard.GetState().IsKeyDown(Keys.D1))
            {
                model = models[0];
                number = 0;
            }
            if (Keyboard.GetState().IsKeyDown(Keys.D2))
            {
                model = models[1];
                number = 1;
            }
            if (Keyboard.GetState().IsKeyDown(Keys.D3))
            {
                model = models[2];
                number = 2;
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
            lightPosition = Vector3.Transform(new Vector3(0, 0, 10),
                Matrix.CreateRotationX(angleL2) * Matrix.CreateRotationY(angleL));

            if (Mouse.GetState().LeftButton == ButtonState.Pressed)
            {
                angle -= (Mouse.GetState().X - preMouse.X) / 100f;
                angle2 += (Mouse.GetState().Y - preMouse.Y) / 100f;
            }
            if (Mouse.GetState().RightButton == ButtonState.Pressed)
            {
                distance += (Mouse.GetState().X - preMouse.X) / 100f;
            }
            if (Mouse.GetState().MiddleButton == ButtonState.Pressed)
            {
                Vector3 ViewRight = Vector3.Transform(Vector3.UnitX,
                Matrix.CreateRotationX(angle2) *
                Matrix.CreateRotationY(angle));
                Vector3 ViewUp = Vector3.Transform(Vector3.UnitY,
                Matrix.CreateRotationX(angle2) *
                Matrix.CreateRotationY(angle));
                cameraTarget -= ViewRight * (Mouse.GetState().X - preMouse.X) / 10f;
                cameraTarget += ViewUp * (Mouse.GetState().Y - preMouse.Y) / 10f;
            }

            preMouse = Mouse.GetState();

            cameraPosition = Vector3.Transform(new Vector3(0, 0, distance),
                Matrix.CreateRotationX(angle2) *
                Matrix.CreateRotationY(angle) *
                Matrix.CreateTranslation(cameraTarget));
            view = Matrix.CreateLookAt(cameraPosition, cameraTarget,
                Vector3.Transform(Vector3.UnitY,
                Matrix.CreateRotationX(angle2) *
                Matrix.CreateRotationY(angle)));

            lightPosition = Vector3.Transform(new Vector3(0, 0, 10),
                Matrix.CreateRotationX(angleL2) * Matrix.CreateRotationY(angleL));

            lightView = Matrix.CreateLookAt(lightPosition, Vector3.Zero,
                Vector3.Transform(
                Vector3.UnitY,
                Matrix.CreateRotationX(angleL2) * Matrix.CreateRotationY(angleL)));
            lightProjection = Matrix.CreatePerspectiveFieldOfView(MathHelper.PiOver2, 1f, 1f, 50f);

            base.Update(gameTime);
        }

        /// <summary>
        /// This is called when the game should draw itself.
        /// </summary>
        /// <param name="gameTime">Provides a snapshot of timing values.</param>
        protected override void Draw(GameTime gameTime)
        {
            /*
            * Phong with no light
            */
            GraphicsDevice.Clear(ClearOptions.Target | ClearOptions.DepthBuffer | ClearOptions.Stencil, Color.Black, 1.0f, 0);
            drawPrePass();

            //Clear the render target
            GraphicsDevice.SetRenderTarget(null);
            prePass = (Texture2D)preRenderTarget;
            
            /* 
            * Standard Phong
            */
            // Set render target
            GraphicsDevice.SetRenderTarget(preRenderTarget);
            GraphicsDevice.Clear(ClearOptions.Target | ClearOptions.DepthBuffer | ClearOptions.Stencil, Color.Black, 1.0f, 0);         
            drawLitScene();
            // Clear render target
            GraphicsDevice.SetRenderTarget(null);
            litScene = (Texture2D)renderTarget;
            
            GraphicsDevice.Clear(ClearOptions.Target | ClearOptions.DepthBuffer | ClearOptions.Stencil, Color.Black, 1.0f, 0);
            
            effect.CurrentTechnique = effect.Techniques[1]; //post-processing effect
            effect.Parameters["prePass"].SetValue(prePass);
            effect.Parameters["lit"].SetValue(litScene);
            effect.Parameters["LightPosition"].SetValue(lightPosition);
            effect.Parameters["Exposure"].SetValue(Exposure);
            effect.Parameters["Decay"].SetValue(Decay);
            effect.Parameters["Density"].SetValue(Density);
            effect.Parameters["Weight"].SetValue(Weight);

            // Pass in screen coordinates for light
            Vector4 lightFinal = new Vector4(lightPosition, 1);
            lightFinal = Vector4.Transform(lightFinal, view * projection);
            lightFinal.X /= lightFinal.W;
            lightFinal.Y /= lightFinal.W;
            lightFinal.Y = -lightFinal.Y + .5f;
            lightFinal.X = lightFinal.X + .5f;
            effect.Parameters["FinalLightPosition"].SetValue(lightFinal);

            using (SpriteBatch sprite = new SpriteBatch(GraphicsDevice))
            {
                sprite.Begin(0, BlendState.AlphaBlend,
                SamplerState.LinearClamp, DepthStencilState.Default, null, effect);
                sprite.Draw(prePass, new Vector2(0, 0), null, Color.White, 0,
                new Vector2(0, 0), 1f, SpriteEffects.None, 1);
                sprite.End();
            }
            
            if (draw)
            {
                spriteBatch.Begin();
                spriteBatch.DrawString(font, "CameraAngle:" + angle, new Vector2(0, 0), Color.White);
                spriteBatch.DrawString(font, "CameraAngle2:" + angle2, new Vector2(0, 15), Color.White);
                spriteBatch.DrawString(font, "LightAngle:" + angleL, new Vector2(0, 30), Color.White);
                spriteBatch.DrawString(font, "LightAngle2:" + angleL2, new Vector2(0, 45), Color.White);
                spriteBatch.DrawString(font, "Exposure:" + Exposure, new Vector2(0, 60), Color.White);
                spriteBatch.DrawString(font, "Density:" + Density, new Vector2(0, 75), Color.White);
                spriteBatch.DrawString(font, "Decay:" + Decay, new Vector2(0, 90), Color.White);
                spriteBatch.DrawString(font, "Weight:" + Weight, new Vector2(0, 105), Color.White);
                spriteBatch.DrawString(font, "Model:" + modelName[number], new Vector2(0, 120), Color.White);
                spriteBatch.DrawString(font, "Ambient:" + Ambient, new Vector2(0, 135), Color.White);
                spriteBatch.DrawString(font, "Shininess:" + shininess, new Vector2(0, 150), Color.White);
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
                spriteBatch.DrawString(font, "Change model 1-3", new Vector2(850, 90), Color.White);
                spriteBatch.DrawString(font, "Increase/Decrease Weight w/W", new Vector2(850, 105), Color.White);
                spriteBatch.DrawString(font, "Increase/Decrease Density t/T", new Vector2(850, 120), Color.White);
                spriteBatch.DrawString(font, "Increase/Decrease Decay d/D", new Vector2(850, 135), Color.White);
                spriteBatch.DrawString(font, "Increase/Decrease Exposure e/E", new Vector2(850, 150), Color.White);
                spriteBatch.DrawString(font, "Increase/Decrease Red r/R", new Vector2(850, 165), Color.White);
                spriteBatch.DrawString(font, "Increase/Decrease Green g/G", new Vector2(850, 180), Color.White);
                spriteBatch.DrawString(font, "Increase/Decrease Blue b/B", new Vector2(850, 195), Color.White);
                spriteBatch.DrawString(font, "Increase/Decrease Shininess s/S:", new Vector2(850, 210), Color.White);
                spriteBatch.End();
            }

            base.Draw(gameTime);
        }
        private void drawPrePass()
        {
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
                        Matrix worldInverseTranspose =
                            Matrix.Transpose(Matrix.Invert(mesh.ParentBone.Transform));
                        effect.Parameters["WorldInverseTranspose"].SetValue(worldInverseTranspose);
                        effect.Parameters["LightPosition"].SetValue(lightPosition);
                        effect.Parameters["CameraPosition"].SetValue(cameraPosition);
                        effect.Parameters["Shininess"].SetValue(shininess);
                        effect.Parameters["AmbientColor"].SetValue(Ambient);
                        effect.Parameters["DiffuseColor"].SetValue(DiffuseColor);
                        effect.Parameters["SpecularColor"].SetValue(specularColor);
                        effect.Parameters["AmbientIntensity"].SetValue(0.0f);
                        effect.Parameters["DiffuseIntensity"].SetValue(0.0f);
                        effect.Parameters["SpecularIntensity"].SetValue(0.0f);
                        pass.Apply();
                        GraphicsDevice.SetVertexBuffer(part.VertexBuffer);
                        GraphicsDevice.Indices = part.IndexBuffer;
                        GraphicsDevice.DrawIndexedPrimitives(PrimitiveType.TriangleList,
                        part.VertexOffset, part.StartIndex, part.PrimitiveCount);
                    }
                }
            }
        }
        private void drawLitScene()
        {
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
                        effect.Parameters["CameraPosition"].SetValue(cameraPosition);
                        Matrix worldInverseTranspose =
                        Matrix.Transpose(Matrix.Invert(mesh.ParentBone.Transform));
                        effect.Parameters["WorldInverseTranspose"].SetValue(worldInverseTranspose);
                        effect.Parameters["LightPosition"].SetValue(lightPosition);
                        effect.Parameters["Shininess"].SetValue(shininess);
                        effect.Parameters["AmbientColor"].SetValue(Ambient);
                        effect.Parameters["DiffuseColor"].SetValue(DiffuseColor);
                        effect.Parameters["SpecularColor"].SetValue(specularColor);
                        effect.Parameters["AmbientIntensity"].SetValue(AmbientIntensity);
                        effect.Parameters["DiffuseIntensity"].SetValue(DiffuseIntensity);
                        effect.Parameters["SpecularIntensity"].SetValue(specularIntensity);
                        pass.Apply();
                        GraphicsDevice.SetVertexBuffer(part.VertexBuffer);
                        GraphicsDevice.Indices = part.IndexBuffer;
                        GraphicsDevice.DrawIndexedPrimitives(PrimitiveType.TriangleList,
                        part.VertexOffset, part.StartIndex, part.PrimitiveCount);
                    }
                }
            }
        }
    }
}