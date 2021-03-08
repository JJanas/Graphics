using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using CPI411.SimpleEngine;

namespace Assignment3
{
    /// <summary>
    /// This is the main type for your game.
    /// </summary>
    public class Assignment3 : Game
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
        int Technique = 2;
        bool draw = false;
        bool help = false;

        Texture2D[] textures = new Texture2D[9];
        string[] techniques = new string[5];
        string[] skyboxTextures = new string[6];
        

        Vector4 Ambient = new Vector4(1.0f, 1.0f, 1.0f, 1.0f);
        float AmbientIntensity = 0.7f;
        Vector4 DiffuseColor = new Vector4(.7f, .7f, .7f, 1.0f);
        Vector3 DiffuseLightDirection = new Vector3(1, 1, 1);
        float DiffuseIntensity = 1.0f;
        Vector4 specularColor = new Vector4(1.0f, 1.0f, 1.0f, 1.0f);
        float specularIntensity = 1.0f;
        float shininess = 10.0f;
        Vector3 etaRatio = new Vector3(1.0f, 1.0f, 1.0f);
        float reflectivity = 0.5f;
        float refractivity = 0.5f;
        float bumpHeight = 9.0f;
        float normalMapRepeatU = 1.0f;
        float normalMapRepeatV = 1.0f;
        Vector2 UVScale = new Vector2(1.0f, 1.0f);
        int MipMap = 1;


        MouseState previousMouseState;

        public Assignment3()
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
            techniques[0] = "Tangent Normal";
            techniques[1] = "World Normal";
            techniques[2] = "Tangent Map";
            techniques[3] = "Reflection";
            techniques[4] = "Refraction";

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

            skyboxTextures[0] = "EnvironmentMaps/nvlobby_new_negx";
            skyboxTextures[1] = "EnvironmentMaps/nvlobby_new_posx";
            skyboxTextures[2] = "EnvironmentMaps/nvlobby_new_negy";
            skyboxTextures[3] = "EnvironmentMaps/nvlobby_new_posy";
            skyboxTextures[4] = "EnvironmentMaps/nvlobby_new_negz";
            skyboxTextures[5] = "EnvironmentMaps/nvlobby_new_posz";

            skybox = new Skybox(skyboxTextures, Content, GraphicsDevice);

            model = Content.Load<Model>("models/Torus");
            effect = Content.Load<Effect>("BumpMap");
            font = Content.Load<SpriteFont>("Font");
            textures[0] = Content.Load<Texture2D>("NormalMaps/art");
            textures[1] = Content.Load<Texture2D>("NormalMaps/BumpTest");
            textures[2] = Content.Load<Texture2D>("NormalMaps/crossHatch");
            textures[3] = Content.Load<Texture2D>("NormalMaps/monkey");
            textures[4] = Content.Load<Texture2D>("NormalMaps/nm");
            textures[5] = Content.Load<Texture2D>("NormalMaps/round");
            textures[6] = Content.Load<Texture2D>("NormalMaps/saint");
            textures[7] = Content.Load<Texture2D>("NormalMaps/science");
            textures[8] = Content.Load<Texture2D>("NormalMaps/square");
            texture = textures[5];

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
                texture = textures[0];
            }
            if (Keyboard.GetState().IsKeyDown(Keys.D2))
            {
                texture = textures[1];
            }
            if (Keyboard.GetState().IsKeyDown(Keys.D3))
            {
                texture = textures[2];
            }
            if (Keyboard.GetState().IsKeyDown(Keys.D4))
            {
                texture = textures[3];
            }
            if (Keyboard.GetState().IsKeyDown(Keys.D5))
            {
                texture = textures[4];
            }
            if (Keyboard.GetState().IsKeyDown(Keys.D6))
            {
                texture = textures[5];
            }
            if (Keyboard.GetState().IsKeyDown(Keys.D7))
            {
                texture = textures[6];
            }
            if (Keyboard.GetState().IsKeyDown(Keys.D8))
            {
                texture = textures[7];
            }
            if (Keyboard.GetState().IsKeyDown(Keys.D9))
            {
                texture = textures[8];
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
                
            }
            if (Keyboard.GetState().IsKeyDown(Keys.F7))
            {
                
            }
            if (Keyboard.GetState().IsKeyDown(Keys.F8))
            {
                
            }
            if (Keyboard.GetState().IsKeyDown(Keys.F9))
            {
                
            }
            if (Keyboard.GetState().IsKeyDown(Keys.F10))
            {
                
            }

            if (Keyboard.GetState().IsKeyDown(Keys.U) && (!Keyboard.GetState().IsKeyDown(Keys.LeftShift) && !Keyboard.GetState().IsKeyDown(Keys.RightShift)))
            {
                normalMapRepeatU += 0.1f;
                UVScale.X = normalMapRepeatU;
            }
            if (Keyboard.GetState().IsKeyDown(Keys.V) && (!Keyboard.GetState().IsKeyDown(Keys.LeftShift) && !Keyboard.GetState().IsKeyDown(Keys.RightShift)))
            {
                normalMapRepeatV += 0.1f;
                UVScale.Y = normalMapRepeatV;
            }
            if (Keyboard.GetState().IsKeyDown(Keys.W) && (!Keyboard.GetState().IsKeyDown(Keys.LeftShift) && !Keyboard.GetState().IsKeyDown(Keys.RightShift)))
            {
                bumpHeight += 0.1f;
            }
            if (Keyboard.GetState().IsKeyDown(Keys.U) && (Keyboard.GetState().IsKeyDown(Keys.LeftShift) || Keyboard.GetState().IsKeyDown(Keys.RightShift)))
            {
                normalMapRepeatU -= 0.1f;
                UVScale.X = normalMapRepeatU;
            }
            if (Keyboard.GetState().IsKeyDown(Keys.V) && (Keyboard.GetState().IsKeyDown(Keys.LeftShift) || Keyboard.GetState().IsKeyDown(Keys.RightShift)))
            {
                normalMapRepeatV -= 0.1f;
                UVScale.Y = normalMapRepeatV;
            }
            if (Keyboard.GetState().IsKeyDown(Keys.W) && (Keyboard.GetState().IsKeyDown(Keys.LeftShift) || Keyboard.GetState().IsKeyDown(Keys.RightShift)))
            {
                bumpHeight -= 0.1f;
            }
            if (Keyboard.GetState().IsKeyDown(Keys.M))
            {
                if(MipMap == 0)
                {
                    MipMap = 1;
                }
                if(MipMap == 1)
                {
                    MipMap = 0;
                }
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

            RasterizerState originalRasterizerState = graphics.GraphicsDevice.RasterizerState;
            RasterizerState rasterizerState = new RasterizerState();
            rasterizerState.CullMode = CullMode.None;
            graphics.GraphicsDevice.RasterizerState = rasterizerState;
            skybox.Draw(view, projection, cameraPosition);
            graphics.GraphicsDevice.RasterizerState = originalRasterizerState;

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
                        effect.Parameters["SpecularColor"].SetValue(specularColor);
                        effect.Parameters["SpecularIntensity"].SetValue(specularIntensity);
                        effect.Parameters["CameraPosition"].SetValue(cameraPosition);
                        effect.Parameters["LightPosition"].SetValue(lightPosition);
                        effect.Parameters["ETARatio"].SetValue(etaRatio);
                        effect.Parameters["Reflectivity"].SetValue(reflectivity);
                        effect.Parameters["Refractivity"].SetValue(refractivity);
                        effect.Parameters["Shininess"].SetValue(shininess);
                        effect.Parameters["normalMap"].SetValue(texture);
                        effect.Parameters["BumpHeight"].SetValue(bumpHeight);
                        effect.Parameters["NormalMapRepeatU"].SetValue(normalMapRepeatU);
                        effect.Parameters["NormalMapRepeatV"].SetValue(normalMapRepeatV);
                        effect.Parameters["UVScale"].SetValue(UVScale);
                        effect.Parameters["MipMap"].SetValue(MipMap);
                        effect.Parameters["environmentMap"].SetValue(skybox.skyBoxTexture);
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
                spriteBatch.DrawString(font, "Refractivity:" + refractivity, new Vector2(0, 75), Color.White);
                spriteBatch.DrawString(font, "BumpHeight:" + bumpHeight, new Vector2(0, 90), Color.White);
                spriteBatch.DrawString(font, "NormalMapRepeatU:" + normalMapRepeatU, new Vector2(0, 105), Color.White);
                spriteBatch.DrawString(font, "NormalMapRepeatV:" + normalMapRepeatV, new Vector2(0, 120), Color.White);
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
                spriteBatch.DrawString(font, "Press w/W to increase/decrease bump height", new Vector2(850, 90), Color.White);
                spriteBatch.DrawString(font, "Press u/U to increase/decrease normal map repetition U", new Vector2(850, 105), Color.White);
                spriteBatch.DrawString(font, "Press v/V to increase/decrease normal map repetition V", new Vector2(850, 120), Color.White);
                spriteBatch.DrawString(font, "Change Shader: F1-10", new Vector2(850, 135), Color.White);
                spriteBatch.End();
            }
            
            base.Draw(gameTime);
        }
    }
}
