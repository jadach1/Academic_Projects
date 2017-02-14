namespace Assignment9.Migrations
{
    using System;
    using System.Data.Entity.Migrations;
    
    public partial class artistmedia : DbMigration
    {
        public override void Up()
        {
            CreateTable(
                "dbo.ArtistMedias",
                c => new
                    {
                        Id = c.Int(nullable: false, identity: true),
                        TimeStamp = c.DateTime(nullable: false),
                        StringId = c.String(nullable: false, maxLength: 100),
                        content = c.Binary(),
                        ContentType = c.String(maxLength: 200),
                        Caption = c.String(nullable: false, maxLength: 100),
                        Artist_Id = c.Int(nullable: false),
                    })
                .PrimaryKey(t => t.Id)
                .ForeignKey("dbo.Artists", t => t.Artist_Id)
                .Index(t => t.Artist_Id);
            
        }
        
        public override void Down()
        {
            DropForeignKey("dbo.ArtistMedias", "Artist_Id", "dbo.Artists");
            DropIndex("dbo.ArtistMedias", new[] { "Artist_Id" });
            DropTable("dbo.ArtistMedias");
        }
    }
}
