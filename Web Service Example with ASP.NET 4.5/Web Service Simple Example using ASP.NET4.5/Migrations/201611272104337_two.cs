namespace Assignment9.Migrations
{
    using System;
    using System.Data.Entity.Migrations;
    
    public partial class two : DbMigration
    {
        public override void Up()
        {
            AddColumn("dbo.Tracks", "Media", c => c.Binary());
            AddColumn("dbo.Tracks", "MediaContentType", c => c.String());
        }
        
        public override void Down()
        {
            DropColumn("dbo.Tracks", "MediaContentType");
            DropColumn("dbo.Tracks", "Media");
        }
    }
}
