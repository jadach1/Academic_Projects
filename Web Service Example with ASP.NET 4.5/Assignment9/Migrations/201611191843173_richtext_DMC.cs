namespace Assignment9.Migrations
{
    using System;
    using System.Data.Entity.Migrations;
    
    public partial class richtext_DMC : DbMigration
    {
        public override void Up()
        {
            AddColumn("dbo.Albums", "Description", c => c.String(maxLength: 2000));
            AddColumn("dbo.Artists", "Profile", c => c.String(maxLength: 2000));
        }
        
        public override void Down()
        {
            DropColumn("dbo.Artists", "Profile");
            DropColumn("dbo.Albums", "Description");
        }
    }
}
