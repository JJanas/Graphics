using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using CPI411.SimpleEngine;

namespace Assignment2
{
    /// <summary>
    /// This is the main type for your game.
    /// </summary>
    public class Assignment2 : Game
    {
        GraphicsDeviceManager graphics;
        SpriteBatch spriteBatch;

        Effect effect;
        SpriteFont font;

        Model model;

        Texture2D texture;
        Skybox skybox;

        Matrix world, view, projection;

        Vector3 cameraOffset = new Vector3(0, 0, 0);
        Vector3 cameraPosition = new Vector3(0, 0, 10);
        Vector3 lightPosition = new Vector3(1.0f, 1.0f, 0.0f);
        float cameraX = 0;
        float cameraY = 0;
        float cameraZ = 20;
        float lightX = 1;
        float lightY = 1;
        float lightZ = 0;
        float angle = 0;
        float angle2 = 0;
        float angle3 = 0;
        float angle4 = 0;
        float offset = 0;
        float offset2 = 0;
        int Technique = 0;
        bool draw = false;
        bool help = false;

        Model[] models = new Model[6];
        string[] techniques = new string[10];
        string[] skyboxTextures = new string[6];


        Vector4 Ambient = new Vector4(0.0f, 0.0f, 0.0f, 1.0f);
        float AmbientIntensity = 0.7f;
        Vector4 DiffuseColor = new Vector4(.7f, .7f, .7f, 1.0f);
        Vector3 DiffuseLightDirection = new Vector3(1, 1, 1);
        float DiffuseIntensity = 1.0f;
        Vector4 specularColor = new Vector4(1.0f, 1.0f, 1.0f, 1.0f);
        float specularIntensity = 1.0f;
        float shininess = 10.0f;
        Vector3 etaRatio = new Vector3(1.0f, 1.0f, 1.0f);
        float reflectivity = 0.5f;
        float fresnelBias = 0.5f;
        float fresnelScale = 0.5f;
        float fresnelPower = 0.5f;


        MouseState previousMouseState;

        public Assignment2()
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
            techniques[0] = "Gouraud";
            techniques[1] = "Phong";
            techniques[2] = "PhongBlinn";
            techniques[3] = "Schlick";
            techniques[4] = "Toon";
            techniques[5] = "HalfLife";
            techniques[6] = "Reflection";
            techniques[7] = "Refraction";
            techniques[8] = "Dispersion";
            techniques[9] = "Fresnel";

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

            skyboxTextures[0] = "EnvironmentMaps/test_negx";
            skyboxTextures[1] = "EnvironmentMaps/test_posx";
            skyboxTextures[2] = "EnvironmentMaps/test_negy";
            skyboxTextures[3] = "EnvironmentMaps/test_posy";
            skyboxTextures[4] = "EnvironmentMaps/test_negz";
            skyboxTextures[5] = "EnvironmentMaps/test_posz";

            skybox = new Skybox(skyboxTextures, Content, GraphicsDevice);

            models[0] = Content.Load<Model>("models/Box");
            models[1] = Content.Load<Model>("models/Sphere");
            models[2] = Content.Load<Model>("models/Torus");
            models[3] = Content.Load<Model>("models/Teapot");
            models[4] = Content.Load<Model>("models/bunny");
            models[5] = Content.Load<Model>("Helicopter");
            model = models[5];
            effect = Content.Load<Effect>("SimpleShading");
            font = Content.Load<SpriteFont>("Font");
            texture = Content.Load<Texture2D>("HelicopterTexture");
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
                model = models[0];
            }
            if (Keyboard.GetState().IsKeyDown(Keys.D2))
            {
                model = models[1];
            }
            if (Keyboard.GetState().IsKeyDown(Keys.D3))
            {
                model = models[2];
            }
            if (Keyboard.GetState().IsKeyDown(Keys.D4))
            {
                model = models[3];
            }
            if (Keyboard.GetState().IsKeyDown(Keys.D5))
            {
                model = models[4];
            }
            if (Keyboard.GetState().IsKeyDown(Keys.D6))
            {
                model = models[5];
            }
            if (Keyboard.GetState().IsKeyDown(Keys.D7))
            {
                skyboxTextures[0] = "EnvironmentMaps/debug_negx";
                skyboxTextures[1] = "EnvironmentMaps/debug_posx";
                skyboxTextures[2] = "EnvironmentMaps/debug_negy";
                skyboxTextures[3] = "EnvironmentMaps/debug_posy";
                skyboxTextures[4] = "EnvironmentMaps/debug_negz";
                skyboxTextures[5] = "EnvironmentMaps/debug_posz";
                skybox = new Skybox(skyboxTextures, Content, GraphicsDevice);
            }
            if (Keyboard.GetState().IsKeyDown(Keys.D8))
            {
                skyboxTextures[0] = "EnvironmentMaps/nvlobby_new_negx";
                skyboxTextures[1] = "EnvironmentMaps/nvlobby_new_posx";
                skyboxTextures[2] = "EnvironmentMaps/nvlobby_new_negy";
                skyboxTextures[3] = "EnvironmentMaps/nvlobby_new_posy";
                skyboxTextures[4] = "EnvironmentMaps/nvlobby_new_negz";
                skyboxTextures[5] = "EnvironmentMaps/nvlobby_new_posz";
                skybox = new Skybox(skyboxTextures, Content, GraphicsDevice);
            }
            if (Keyboard.GetState().IsKeyDown(Keys.D9))
            {
                skyboxTextures[0] = "EnvironmentMaps/grandcanyon_negx";
                skyboxTextures[1] = "EnvironmentMaps/grandcanyon_posx";
                skyboxTextures[2] = "EnvironmentMaps/grandcanyon_negy";
                skyboxTextures[3] = "EnvironmentMaps/grandcanyon_posy";
                skyboxTextures[4] = "EnvironmentMaps/grandcanyon_negz";
                skyboxTextures[5] = "EnvironmentMaps/grandcanyon_posz";
                skybox = new Skybox(skyboxTextures, Content, GraphicsDevice);
            }
            if (Keyboard.GetState().IsKeyDown(Keys.D0))
            {
                skyboxTextures[0] = "EnvironmentMaps/starfield_lf";
                skyboxTextures[1] = "EnvironmentMaps/starfield_rt";
                skyboxTextures[2] = "EnvironmentMaps/starfield_dn";
                skyboxTextures[3] = "EnvironmentMaps/starfield_up";
                skyboxTextures[4] = "EnvironmentMaps/starfield_bk";
                skyboxTextures[5] = "EnvironmentMaps/starfield_ft";
                skybox = new Skybox(skyboxTextures, Content, GraphicsDevice);
            }
            /*Shader controls*/
            if (Keyboard.GetState().IsKeyDown(Keys.F1))
            {
                Technique = 0;
            }
            if (Keyboard.GetState().IsKeyDown(Keys.F2))
            {
                Technique = 1;
            }
            if (Keyboard.GetState().IsKeyDown(Keys.F3))
            {
                Technique = 2;
            }
            if (Keyboard.GetState().IsKeyDown(Keys.F4))
            {
                Technique = 3;
            }
            if (Keyboard.GetState().IsKeyDown(Keys.F5))
            {
                Technique = 4;
            }
            if (Keyboard.GetState().IsKeyDown(Keys.F6))
            {
                Technique = 5;
            }
            if (Keyboard.GetState().IsKeyDown(Keys.F7))
            {
                Technique = 6;
            }
            if (Keyboard.GetState().IsKeyDown(Keys.F8))
            {
                Technique = 7;
            }
            if (Keyboard.GetState().IsKeyDown(Keys.F9))
            {
                Technique = 8;
            }
            if (Keyboard.GetState().IsKeyDown(Keys.F10))
            {
                Technique = 9;
            }
            if (Keyboard.GetState().IsKeyDown(Keys.OemMinus))
            {
                reflectivity -= .01f;
            }
            if (Keyboard.GetState().IsKeyDown(Keys.OemPlus))
            {
                reflectivity += .01f;
            }
            if (Keyboard.GetState().IsKeyDown(Keys.R) && !(Keyboard.GetState().IsKeyDown(Keys.LeftShift) || Keyboard.GetState().IsKeyDown(Keys.RightShift)))
            {
                etaRatio.X += .1f;
            }
            if (Keyboard.GetState().IsKeyDown(Keys.G) && !(Keyboard.GetState().IsKeyDown(Keys.LeftShift) || Keyboard.GetState().IsKeyDown(Keys.RightShift)))
            {
                etaRatio.Y += .1f;
            }
            if (Keyboard.GetState().IsKeyDown(Keys.B) && !(Keyboard.GetState().IsKeyDown(Keys.LeftShift) || Keyboard.GetState().IsKeyDown(Keys.RightShift)))
            {
                etaRatio.Z += .1f;
            }
            if (Keyboard.GetState().IsKeyDown(Keys.R) && (Keyboard.GetState().IsKeyDown(Keys.LeftShift) || Keyboard.GetState().IsKeyDown(Keys.RightShift)))
            {
                etaRatio.X -= .1f;
            }
            if (Keyboard.GetState().IsKeyDown(Keys.G) && (Keyboard.GetState().IsKeyDown(Keys.LeftShift) || Keyboard.GetState().IsKeyDown(Keys.RightShift)))
            {
                etaRatio.Y -= .1f;
            }
            if (Keyboard.GetState().IsKeyDown(Keys.B) && (Keyboard.GetState().IsKeyDown(Keys.LeftShift) || Keyboard.GetState().IsKeyDown(Keys.RightShift)))
            {
                etaRatio.Z -= .1f;
            }
            if (Keyboard.GetState().IsKeyDown(Keys.Q) && !(Keyboard.GetState().IsKeyDown(Keys.LeftShift) || Keyboard.GetState().IsKeyDown(Keys.RightShift)))
            {
                fresnelPower += .1f;
            }
            if (Keyboard.GetState().IsKeyDown(Keys.W) && !(Keyboard.GetState().IsKeyDown(Keys.LeftShift) || Keyboard.GetState().IsKeyDown(Keys.RightShift)))
            {
                fresnelScale += .1f;
            }
            if (Keyboard.GetState().IsKeyDown(Keys.E) && !(Keyboard.GetState().IsKeyDown(Keys.LeftShift) || Keyboard.GetState().IsKeyDown(Keys.RightShift)))
            {
                fresnelBias += .1f;
            }
            if (Keyboard.GetState().IsKeyDown(Keys.Q) && (Keyboard.GetState().IsKeyDown(Keys.LeftShift) || Keyboard.GetState().IsKeyDown(Keys.RightShift)))
            {
                fresnelPower -= .1f;
            }
            if (Keyboard.GetState().IsKeyDown(Keys.W) && (Keyboard.GetState().IsKeyDown(Keys.LeftShift) || Keyboard.GetState().IsKeyDown(Keys.RightShift)))
            {
                fresnelScale -= .1f;
            }
            if (Keyboard.GetState().IsKeyDown(Keys.E) && (Keyboard.GetState().IsKeyDown(Keys.LeftShift) || Keyboard.GetState().IsKeyDown(Keys.RightShift)))
            {
                fresnelBias -= .1f;
            }
            /*Color
            if (Keyboard.GetState().IsKeyDown(Keys.L))
            {
                Ambient.W += 0.02f;
                DiffuseColor.W += 0.02f;
                specularColor.W += 0.02f;
            }
            if (Keyboard.GetState().IsKeyDown(Keys.R))
            {
                Ambient.X += 0.02f;
                DiffuseColor.X += 0.02f;
                specularColor.X += 0.02f;
            }
            if (Keyboard.GetState().IsKeyDown(Keys.G))
            {
                Ambient.Y += 0.02f;
                DiffuseColor.Y += 0.02f;
                specularColor.Y += 0.02f;
            }
            if (Keyboard.GetState().IsKeyDown(Keys.B))
            {
                Ambient.Z += 0.02f;
                DiffuseColor.Z += 0.02f;
                specularColor.Z += 0.02f;
            }
            if (Keyboard.GetState().IsKeyDown(Keys.L) && (Keyboard.GetState().IsKeyDown(Keys.LeftShift) || Keyboard.GetState().IsKeyDown(Keys.RightShift)))
            {
                Ambient.W -= 0.02f;
                DiffuseColor.W -= 0.02f;
                specularColor.W -= 0.02f;
            }
            if (Keyboard.GetState().IsKeyDown(Keys.R) && (Keyboard.GetState().IsKeyDown(Keys.LeftShift) || Keyboard.GetState().IsKeyDown(Keys.RightShift)))
            {
                Ambient.X -= 0.02f;
                DiffuseColor.X -= 0.02f;
                specularColor.X -= 0.02f;
            }
            if (Keyboard.GetState().IsKeyDown(Keys.G) && (Keyboard.GetState().IsKeyDown(Keys.LeftShift) || Keyboard.GetState().IsKeyDown(Keys.RightShift)))
            {
                Ambient.Y -= 0.02f;
                DiffuseColor.Y -= 0.02f;
                specularColor.Y -= 0.02f;

            }
            if (Keyboard.GetState().IsKeyDown(Keys.B) && (Keyboard.GetState().IsKeyDown(Keys.LeftShift) || Keyboard.GetState().IsKeyDown(Keys.RightShift)))
            {
                Ambient.Z -= 0.02f;
                DiffuseColor.Z -= 0.02f;
                specularColor.Z -= 0.02f;
            }
            */
            /*Specular
            if (Keyboard.GetState().IsKeyDown(Keys.OemPlus))
            {
                specularIntensity += 0.1f;
            }
            if (Keyboard.GetState().IsKeyDown(Keys.OemMinus))
            {
                specularIntensity -= 0.1f;
            }
            */
            /*Shininess
            if (Keyboard.GetState().IsKeyDown(Keys.LeftControl) && Keyboard.GetState().IsKeyDown(Keys.OemPlus))
            {
                shininess += 0.1f;
            }
            if (Keyboard.GetState().IsKeyDown(Keys.LeftControl) && Keyboard.GetState().IsKeyDown(Keys.OemMinus))
            {
                shininess -= 0.1f;
            }
            */
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
            GraphicsDevice.BlendState = BlendState.Opaque;
            GraphicsDevice.DepthStencilState = new DepthStencilState();

            RasterizerState originalRasterizerState = graphics.GraphicsDevice.RasterizerState;
            RasterizerState rasterizerState = new RasterizerState();
            rasterizerState.CullMode = CullMode.None;
            graphics.GraphicsDevice.RasterizerState = rasterizerState;
            skybox.Draw(view, projection, cameraPosition);
            graphics.GraphicsDevice.RasterizerState = originalRasterizerState;
            DrawModelWithEffect();

            effect.CurrentTechnique = effect.Techniques[Technique];
            foreach (EffectPass pass in effect.CurrentTechnique.Passes)
            {
                foreach (ModelMesh mesh in model.Meshes)
                {
                    foreach (ModelMeshPart part in mesh.MeshParts)
                    {
                        effect.Parameters["World"].SetValue(mesh.ParentBone.Transform);
                        effect.Parameters["View"].SetValue(view);
                        effect.Parameters["Projection"].SetValue(projection);
                        effect.Parameters["AmbientColor"].SetValue(Ambient);
                        effect.Parameters["AmbientIntensity"].SetValue(AmbientIntensity);
                        effect.Parameters["DiffuseColor"].SetValue(DiffuseColor);
                        effect.Parameters["DiffuseIntensity"].SetValue(DiffuseIntensity);
                        effect.Parameters["DiffuseLightDirection"].SetValue(DiffuseLightDirection);
                        effect.Parameters["SpecularColor"].SetValue(specularColor);
                        effect.Parameters["SpecularIntensity"].SetValue(specularIntensity);
                        effect.Parameters["Shininess"].SetValue(shininess);
                        effect.Parameters["CameraPosition"].SetValue(cameraPosition);
                        effect.Parameters["LightPosition"].SetValue(lightPosition);
                        effect.Parameters["ETARatio"].SetValue(etaRatio);
                        effect.Parameters["Reflectivity"].SetValue(reflectivity);
                        effect.Parameters["FresnelBias"].SetValue(fresnelBias);
                        effect.Parameters["FresnelScale"].SetValue(fresnelScale);
                        effect.Parameters["FresnelPower"].SetValue(fresnelPower);

                        Matrix worldInverseTranspose =
                            Matrix.Transpose(Matrix.Invert(mesh.ParentBone.Transform));
                        effect.Parameters["WorldInverseTranspose"].SetValue(worldInverseTranspose);

                        pass.Apply();

                        GraphicsDevice.SetVertexBuffer(part.VertexBuffer);
                        GraphicsDevice.Indices = part.IndexBuffer;

                        GraphicsDevice.DrawIndexedPrimitives(
                        PrimitiveType.TriangleList, part.VertexOffset, 0,
                        part.NumVertices, part.StartIndex, part.PrimitiveCount);

                    }
                }
            }
            if (draw)
            {
                spriteBatch.Begin();
                spriteBatch.DrawString(font, "CameraAngle:" + angle, new Vector2(0, 0), Color.White);
                spriteBatch.DrawString(font, "CameraAngle2:" + angle2, new Vector2(0, 15), Color.White);
                spriteBatch.DrawString(font, "LightAngle:" + angle3, new Vector2(0, 30), Color.White);
                spriteBatch.DrawString(font, "LightAngle2:" + angle4, new Vector2(0, 45), Color.White);
                spriteBatch.DrawString(font, "Reflectivity:" + reflectivity, new Vector2(0, 60), Color.White);
                spriteBatch.DrawString(font, "FrenselBias" + fresnelBias, new Vector2(0, 75), Color.White);
                spriteBatch.DrawString(font, "FrenselScale:" + fresnelScale, new Vector2(0, 90), Color.White);
                spriteBatch.DrawString(font, "FrenselPower:" + fresnelPower, new Vector2(0, 105), Color.White);
                spriteBatch.DrawString(font, "etaRatio:" + etaRatio, new Vector2(0, 120), Color.White);
                spriteBatch.DrawString(font, "Shader:" + techniques[Technique], new Vector2(0, 135), Color.White);
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
                spriteBatch.DrawString(font, "Increase/Decrease eta red: r/R", new Vector2(850, 90), Color.White);
                spriteBatch.DrawString(font, "Increase/Decrease eta green: g/G", new Vector2(850, 105), Color.White);
                spriteBatch.DrawString(font, "Increase/Decrease eta blue: b/B", new Vector2(850, 120), Color.White);
                spriteBatch.DrawString(font, "Increase/Decrease Reflectivity: +/-", new Vector2(850, 135), Color.White);
                spriteBatch.DrawString(font, "Increase/Decrease Fresnel Power: q/Q", new Vector2(850, 150), Color.White);
                spriteBatch.DrawString(font, "Increase/Decrease Fresnel Scale: w/W", new Vector2(850, 165), Color.White);
                spriteBatch.DrawString(font, "Increase/Decrease Fresnel Bias: e/E", new Vector2(850, 180), Color.White);
                spriteBatch.DrawString(font, "Change Shader: F1-10", new Vector2(850, 195), Color.White);
                spriteBatch.DrawString(font, "Change Model: 1-6", new Vector2(850, 210), Color.White);
                spriteBatch.DrawString(font, "Change Skybox: 7-0", new Vector2(850, 225), Color.White);
                spriteBatch.End();
            }
            void DrawModelWithEffect()
            {
                effect.CurrentTechnique = effect.Techniques[Technique];
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
                            effect.Parameters["decalMap"].SetValue(texture);
                            effect.Parameters["environmentMap"].SetValue(skybox.skyBoxTexture);

                            pass.Apply();
                            GraphicsDevice.SetVertexBuffer(part.VertexBuffer);
                            GraphicsDevice.Indices = part.IndexBuffer;

                            GraphicsDevice.DrawIndexedPrimitives(
                                PrimitiveType.TriangleList, part.VertexOffset, 0,
                                part.NumVertices, part.StartIndex, part.PrimitiveCount);
                        }
                    }
                }
            }

            base.Draw(gameTime);
        }
    }
}